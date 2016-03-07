#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <stdlib.h>

#include "camera.h"
#include "mpi_manager.h"
#include "utilities.h"

using namespace std;

Camera::Camera() :
  _output_filename   ("raytracer.jpg"),
  _resolution_x      (750),
  _resolution_y      (500),

  _focal_length      (35), //35mm lens
  _film_size_diagonal(sqrt(36*36 + 24*24)), //full-frame film 36mm x 24mm
  _crop_factor       (1),

  _exposure_range    (-1),
  _exposure          (0),
  _gamma             (1),

  _aperture          (5.6),
  _depth_of_field    (false),

  _colorization      ("rgb"),
  _vignette          (0),

  _position          (Vector()),
  _viewing_direction (Vector(0, 1, 0)),
  _top_direction     (Vector(0, 0, 1)),

  _summary_time(0),
  _summary_mpix(0),
  _summary_pics(0),

  _display_progress(true),
  _display_summary(true)
{
}

void Camera::parse(istream &in)
{
  string command;
  string rest_of_line;

  while(in >> command && getline(in, rest_of_line)) {
    display_progress("Loading", 0);
    stringstream stream(rest_of_line);

    if(command.size() >= 2 && command.substr(0,2) == "//") {
      continue;
    }
    else if(command == "camera") {

      string command2;
      stream >> command2;

      if(command2 == "aperture") {
        char c;
        stream >> c;
        parser_assert_command(string(1, c), "f");
        stream >> c;
        parser_assert_command(string(1, c), "/");
        stream >> _aperture;
      }
      else if(command2 == "colorization") {
        stream >> _colorization;
      }
      else if(command2 == "exposure_range") {
        stream >> _exposure_range;
      }
      else if(command2 == "crop_factor") {
        stream >> _crop_factor;
      }
      else if(command2 == "exposure") {
        stream >> _exposure;
      }
      else if(command2 == "focal_length") {
        stream >> _focal_length;
      }
      else if(command2 == "focus") {
        stream >> _focus;
        _depth_of_field = true;
      }
      else if(command2 == "gamma") {
        stream >> _gamma;
      }
      else if(command2 == "noise") {
        stream >> _noise;
      }
      else if(command2 == "position") {
        stream >> _position;
      }
      else if(command2 == "resolution") {
        stream >> _resolution_x >> _resolution_y;
      }
      else if(command2 == "take_picture") {
        take_picture();
      }
      else if(command2 == "top_direction") {
        stream >> _top_direction;
      }
      else if(command2 == "output_filename") {
        stream >> _output_filename;
      }
      else if(command2 == "viewing_direction") {
        stream >> _viewing_direction;
      }
      else if(command2 == "vignette") {
        stream >> _vignette;
      }
      else {
        parser_error_unknown_command(command2);
      }

    }
    else if(command == "display") {
      string command2;
      stream >> command2;
      if(command2 == "progress") {
        stream >> _display_progress;
      }
      else if(command2 == "summary") {
        stream >> _display_summary;
      }
      else {
        parser_error_unknown_command(command2);
      }
    }
    else if(command == "ray_diagram") {
      _ray_diagram.parse(rest_of_line);
    }
    else if(command == "quit") {
      break;
    }
    else {
      if(mpi.size() > 1) {
        for(int i=1; i<mpi.size(); i++) {
          mpi.send_order(ORDER_PARSE, i);
          mpi.send_string(command + rest_of_line, i);
        }
      }
      else {
        _scene.parse(command + rest_of_line);
      }
    }
  }

  //quit
  display_progress("Done", 1);
  cout << endl;

  if(_display_summary) {
    display_summary();
  }

  for(int i=1; i<mpi.size(); i++) {
    mpi.send_order(ORDER_QUIT, i);
  }
}

void Camera::take_picture()
{
  long t0 = get_time();

  rename_previous_output();
  initialize();
  shutter();
  develop();
  _film.save(_output_filename.c_str(), 0, 100);
  if(_ray_diagram.enabled()) {
    _ray_diagram.save();
  }

  _summary_time += 1.e-6 * (get_time() - t0);
  _summary_mpix += 1.e-6 * _resolution_x * _resolution_y;
  _summary_pics ++;
}

void Camera::rename_previous_output() const
{
  if(file_exists(_output_filename)) {
    for(int i=1; ; i++) {
      size_t found = _output_filename.find_last_of('.');

      stringstream stream;
      stream << _output_filename.substr(0, found) << i << _output_filename.substr(found);
      string new_filename = stream.str();


      if(!file_exists(new_filename)) {
        rename(_output_filename.c_str(), new_filename.c_str());
        break;
      }
    }
  }
}

void Camera::initialize()
{
  //normalize and orthogonalize directions
  _viewing_direction.normalize();
  _top_direction -= (_viewing_direction * _top_direction) * _viewing_direction;
  _top_direction.normalize();
  if( is_equal(0, _viewing_direction.norm()) )
    display_error("Viewing_direction = 0.");
  if( is_equal(0, _top_direction.norm()) )
    display_error("Top_direction = 0.");

  //assert min resolution
  if(_resolution_x <= 1 || _resolution_y <= 1)
    display_error("Resolution has to be >= 2.");
  _film = QImage(_resolution_x, _resolution_y, QImage::Format_RGB32);

  //resize matrix of light_beams
  _light_beams.resize(_resolution_x);
  for(int i=0; i<_resolution_x; i++) {
    _light_beams[i].resize(_resolution_y);
  }

  //CALCULATE FILM ORIENTATION AND DIMENSION
  //For our Raytracing optic we imagine a film frame with a grid of resolution_x * resolution_y pixels.
  //We imagine that the film frame is placed the focal_length in front of the camera position.
  //The initial_rays are going from the camera position through the corresponding pixels on the frame.
  //In contrast to photography the film is placed in front of the camera instead of behind the lens,
  //however terms like film size or focal length are dealt in equal mesure.
  double resolution_diagonal = sqrt(pow(_resolution_x-1, 2) + pow(_resolution_y-1, 2));
  double film_size_x = (_resolution_x-1) / resolution_diagonal * _film_size_diagonal / _crop_factor;
  double film_size_y = (_resolution_y-1) / resolution_diagonal * _film_size_diagonal / _crop_factor;

  Vector right_direction = - (_top_direction ^ _viewing_direction).normalized();

  _film_top_left_direction  = _viewing_direction * _focal_length;
  _film_top_left_direction -= right_direction * (film_size_x / 2);
  _film_top_left_direction += _top_direction * (film_size_y / 2);

  _film_dx = right_direction * (film_size_x / (_resolution_x-1));
  _film_dy = -_top_direction * (film_size_y / (_resolution_y-1));

  //scene
  if(_ray_diagram.enabled()) {
    _ray_diagram.init(_position, _viewing_direction, _top_direction, _resolution_x, _resolution_y);
    _scene.set_ray_diagram(&_ray_diagram);
  }
  if(mpi.size() == 1) {
    _scene.init();
  }
  else {
    for(int i=1; i<mpi.size(); i++) {
      mpi.send_order(ORDER_INIT, i);
      mpi.send_int(_resolution_x, i);
      mpi.send_vector(_position, i);
      mpi.send_vector(_film_top_left_direction, i);
      mpi.send_vector(_film_dx, i);
      mpi.send_vector(_film_dy, i);
    }
  }
}

void Camera::shutter()
{
  if(mpi.size() == 1) {
    for(int j=0; j<_resolution_y; j++) {
      for(int i=0; i<_resolution_x; i++) {

        if(_ray_diagram.enabled()) {
          _ray_diagram.set_pixel_in_progress(i, j);
        }

        Vector v = _film_top_left_direction + j*_film_dy + i*_film_dx;
        _light_beams[i][j] = _scene.raytracer(Ray(_position, v));

      }
      display_progress("RayTracing", double(j)/_resolution_y);
    }
  }
  else {
    vector<int> queue;
    for(int i=1; i<mpi.size(); i++) {
      queue.push_back(i);
    }
    int sent = 0;
    int received = 0;
    while(true) {

      while(queue.size() && sent < _resolution_y) {
        mpi.send_order(ORDER_RAYTRACE, queue[0]);
        mpi.send_int(sent, queue[0]);
        queue.erase(queue.begin());
        sent++;
        display_progress("RayTracing", double(sent)/_resolution_y);
      }

      if(received == _resolution_y) {
        break;
      }
      else {
        int slave;
        mpi.recv_int(slave, MPI_ANY_SOURCE);
        int row;
        mpi.recv_int(row, slave);
        for(int i=0; i<_resolution_x; i++) {
          mpi.recv_light_beam(_light_beams[i][row], slave);
        }
        queue.push_back(slave);
        received++;
      }

    }
  }
}

void Camera::develop()
{
  double lum_max=0;

  if(_depth_of_field) {
    // blur_diameter / | distance - _focus |  =  (_focal_length/_aperture) / _focus
    // blur_diameter_on_film / blur_diameter  =  _focal_length / distance
    const vector< vector< LightBeam > > beams_copy = _light_beams;
    for(int i=0; i<_resolution_x; i++) {
      for(int j=0; j<_resolution_y; j++) {
        double distance = beams_copy[i][j].depth();
        double blur_diameter = fabs(distance - _focus) * (_focal_length/1000) / _aperture / _focus;
        double blur_diameter_on_film = blur_diameter * (_focal_length/1000) / distance;
        int blur_radius_pixel = blur_diameter_on_film / (_film_dx.norm()/1000) / 2;

        int N = 0;
        _light_beams[i][j] = LightBeam();
        for(int a = i - blur_radius_pixel; a <= i + blur_radius_pixel; a++) {
          for(int b = j - blur_radius_pixel; b <= j + blur_radius_pixel; b++) {
            if(a>=0 && b>=0 && a<_resolution_x && b<_resolution_y && (a-i)*(a-i)+(b-j)*(b-j) <= blur_radius_pixel*blur_radius_pixel) {
              _light_beams[i][j] += beams_copy[a][b];
              N++;
            }
          }
        }
        _light_beams[i][j] /= N;

      }
      display_progress("Depth of Field", double(i)/_resolution_x);
    }
  }

  if(_colorization == "bw") {
    for(int i=0; i<_resolution_x; i++) {
      for(int j=0; j<_resolution_y; j++) {
        double avg = (_light_beams[i][j].red() + _light_beams[i][j].green() + _light_beams[i][j].blue()) / 3; 
        _light_beams[i][j] = LightBeam(avg, avg, avg);
      }
      display_progress("Black White", double(i)/_resolution_x);
    }
  }

  if(!is_equal(_vignette, 0)) {
    double diag = sqrt( pow(_resolution_x, 2) + pow(_resolution_y, 2) );
    for(int i=0; i<_resolution_x; i++) {
      for(int j=0; j<_resolution_y; j++) {
        double offset_i = i - _resolution_x / 2.;
        double offset_j = j - _resolution_y / 2.;
        double relative_offset = sqrt( pow(offset_i, 2) + pow(offset_j, 2) ) / (diag/2);
        _light_beams[i][j] *= pow(2, _vignette * relative_offset);
      }
      display_progress("Vignette", double(i)/_resolution_x);
    }
  }

  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      if(_light_beams[i][j].luminosity() > lum_max) {
        lum_max = _light_beams[i][j].luminosity();
      }
    }
  }

  //calculate exposure range
  _range_max = lum_max * pow(2, -_exposure);
  if(_exposure_range < 0)
    _range_min = 0;
  else
    _range_min = _range_max * pow(2, - _exposure_range);
  _range_abs = _range_max - _range_min;


  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      double faktor = 255 / lum_max;

      _film.setPixel(i, j, qRgb(develop_beam(_light_beams[i][j].red()  ),
                                develop_beam(_light_beams[i][j].green()),
                                develop_beam(_light_beams[i][j].blue() ) ));
    }
    display_progress("Develop", double(i)/_resolution_x);
  }
}

int Camera::develop_beam(double l)
{
  double color_relative;

  if(is_equal(_range_abs, 0)) {
    color_relative = 0;
  }
  else if(l <= _range_min) {
    color_relative = 0;
  }
  else {
    color_relative = (l-_range_min) / _range_abs;
  }

  if(!is_equal(_noise, 0)) {
    color_relative += f_rand(0, _noise/_range_max);
  }

  if(color_relative > 1) {
    color_relative = 1;
  }

  return 255 * pow(color_relative, _gamma);
}

void Camera::display_summary()
{
  cout.precision(2);
  cout << "Pictures: " << _summary_pics << "     ";
  cout << "Nodes: " << mpi.size() << "     ";
  cout << "Total: " << _summary_mpix << " MPix     ";
  cout << "Time: " << _summary_time << " s     ";
  cout << "Speed: " << _summary_mpix/_summary_time << " MPix/s" << endl;
}

void Camera::display_progress(const string &name, double progress)
{
  static string _name;
  static double _progress;
  const int width_text = 30;
  const int width_bar = 45;
  //checking if display is necessary
  if(!_display_progress)
    return;
  if(_name == name && is_greater(_progress + 1./width_bar, progress))
    return;
  _name = name;
  _progress = progress;

  cout << "\r" << name;
  for(int i=name.size(); i<width_text; i++)
    cout << ' ';
  cout << '|';
  for(int i=0; i<width_bar; i++) {
    if(i < progress*width_bar)
      cout << '-';
    else
      cout << ' ';
  }
  cout << '|';
  if(progress > 0) {
    cout << " " << int(progress * 100) << "%";
  }
  cout << "     ";
  cout.flush();
}

