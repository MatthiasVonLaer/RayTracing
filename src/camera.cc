#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <stdlib.h>

#include "camera.h"
#include "mpi_manager.h"
#include "polygon.h"
#include "utilities.h"

using namespace std;

Camera::Camera(Display &display) :
  _output_filename   ("raytracer.jpg"),
  _resolution_x      (750),
  _resolution_y      (500),

  _film_size_diagonal(sqrt(.036*.036 + .024*.024)), //full-frame film 36mm x 24mm
  _crop_factor       (1),

  _exposure_range    (-1),
  _exposure          (0),
  _gamma             (1),

  _depth_of_field    (false),
  _diffraction       (false),

  _colorization      ("rgb"),
  _vignette          (0),

  _position          (Vector()),
  _viewing_direction (Vector(0, 1, 0)),
  _top_direction     (Vector(0, 0, 1)),

  _display(display)
{
}

void Camera::parse(const string &command, istream &stream)
{
  if(command == "aperture") {
    char c;
    stream >> c;
    parser_assert_command(string(1, c), "f");
    stream >> c;
    parser_assert_command(string(1, c), "/");
    double aperture;
    stream >> aperture;
    _lens.set_aperture(aperture);
  }
  else if(command == "aperture_blades") {
    int blades;
    stream >> blades;
    _lens.set_blades(blades);
  }
  else if(command == "colorization") {
    stream >> _colorization;
  }
  else if(command == "diffraction") {
    stream >> _diffraction;
  }
  else if(command == "exposure_range") {
    stream >> _exposure_range;
  }
  else if(command == "crop_factor") {
    stream >> _crop_factor;
  }
  else if(command == "exposure") {
    stream >> _exposure;
  }
  else if(command == "focal_length") {
    double focal_length;
    stream >> focal_length;
    _lens.set_focal_length(focal_length / 1000);
  }
  else if(command == "focus") {
    double focus;
    stream >> focus;
    _lens.set_focus(focus);
    _depth_of_field = true;
  }
  else if(command == "gamma") {
    stream >> _gamma;
  }
  else if(command == "noise") {
    stream >> _noise;
  }
  else if(command == "position") {
    stream >> _position;
  }
  else if(command == "resolution") {
    stream >> _resolution_x >> _resolution_y;
  }
  else if(command == "top_direction") {
    stream >> _top_direction;
  }
  else if(command == "output_filename") {
    stream >> _output_filename;
  }
  else if(command == "viewing_direction") {
    stream >> _viewing_direction;
  }
  else if(command == "vignette") {
    stream >> _vignette;
  }
  else {
    parser_error_unknown_command(command);
  }
}

void Camera::take_picture(const Scene *scene)
{
  _display.start_timer();

  rename_previous_output();
  shutter(scene);
  develop();
  _film.save(_output_filename.c_str(), 0, 100);

  _display.stop_timer();
  _display.add_mpix(_resolution_x * _resolution_y);
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

  _film_top_left_direction  = _viewing_direction * _lens.focal_length();
  _film_top_left_direction -= right_direction * (film_size_x / 2);
  _film_top_left_direction += _top_direction * (film_size_y / 2);

  _film_dx = right_direction * (film_size_x / (_resolution_x-1));
  _film_dy = -_top_direction * (film_size_y / (_resolution_y-1));
}

void Camera::shutter(const Scene *scene)
{
  if(mpi.size() == 1) {
    for(int j=0; j<_resolution_y; j++) {
      for(int i=0; i<_resolution_x; i++) {

        _shutter_pixel_x = i;
        _shutter_pixel_y = j;

        Vector v = _film_top_left_direction + j*_film_dy + i*_film_dx;
        _light_beams[i][j] = scene->raytracer(Ray(_position, v));

      }
      _display.progress("RayTracing", double(j)/_resolution_y);
    }
  }

  else {
    for(int i=1; i<mpi.size(); i++) {
      mpi.send_order(ORDER_CAMERA_DATA, i);
      mpi.send_int(_resolution_x, i);
      mpi.send_vector(_position, i);
      mpi.send_vector(_film_top_left_direction, i);
      mpi.send_vector(_film_dx, i);
      mpi.send_vector(_film_dy, i);
    }

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
        _display.progress("RayTracing", double(sent)/_resolution_y);
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
  if(_diffraction) {
    diffraction();
  }

  if(_depth_of_field) {
    depth_of_field();
  }

  if(_colorization == "bw") {
    black_white();
  }

  if(!is_equal(_vignette, 0)) {
    vignette();
  }

  double lum_max = maximal_luminosity();

  //calculate exposure range
  double range_max = lum_max * pow(2, -_exposure);
  double range_min;
  if(_exposure_range < 0)
    range_min = 0;
  else
    range_min = range_max * pow(2, - _exposure_range);


  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      double faktor = 255 / lum_max;

      _film.setPixel(i, j, qRgb(develop_beam(_light_beams[i][j].red()  , range_min, range_max),
            develop_beam(_light_beams[i][j].green(), range_min, range_max),
            develop_beam(_light_beams[i][j].blue() , range_min, range_max) ));
    }
    _display.progress("Develop", double(i)/_resolution_x);
  }
}

void Camera::diffraction()
{
  double lum_max = maximal_luminosity();

  vector< vector< complex<double> > > diffracted_beams;
  diffracted_beams.resize(_resolution_x);
  for(int i=0; i<_resolution_x; i++) {
    diffracted_beams[i].resize(_resolution_y);
    for(int j=0; j<_resolution_y; j++) {
      diffracted_beams[i][j] = 0;
    }
  }
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Diffraction", double(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {
      if(_light_beams[i][j].luminosity() < .8 * lum_max) {
        diffracted_beams[i][j] += _light_beams[i][j].luminosity();
        continue;
      }

      int n = 50;
      double ds = _film_dx.norm();
      for(int a=-n; a<=n; a++) {
        for(int b=-n; b<=n; b++) {
          if(i+a>=0 && j+b>=0 && i+a<_resolution_x && j+b<_resolution_y) {
            diffracted_beams[i+a][j+b] += _light_beams[i][j].luminosity() *_lens.diffraction_pattern(a*ds, b*ds);
          }
        }
      }

    }
  }
  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      double l = norm(diffracted_beams[i][j]);
      _light_beams[i][j] = LightBeam(l, l, l);
    }
  }
}

double Camera::maximal_luminosity()
{
  double lum_max = 0;
  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      if(_light_beams[i][j].luminosity() > lum_max) {
        lum_max = _light_beams[i][j].luminosity();
      }
    }
  }
  return lum_max;
}

void Camera::depth_of_field()
{
  // blur_diameter / | distance - _focus |  =  (_focal_length/_aperture) / _focus
  // blur_diameter_on_film / blur_diameter  =  _focal_length / distance
  const vector< vector< LightBeam > > beams_copy = _light_beams;
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Depth of Field", double(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {
      double blur_diameter = _lens.blur_diameter( beams_copy[i][j].depth() );
      int blur_radius_pixel = blur_diameter / (_film_dx.norm()) / 2;
      if(blur_radius_pixel < 0) {
        cout << blur_radius_pixel << endl;
        blur_radius_pixel = 0;
      }

      int N = 0;
      _light_beams[i][j] = LightBeam();
      Polygon polygon = _lens.aperture_shape(blur_radius_pixel);
      for(int n=0; n<polygon.size(); n++) {
        int a = i+polygon.x(n);
        int b = j+polygon.y(n);
        if(a>=0 && b>=0 && a<_resolution_x && b<_resolution_y) {
          _light_beams[i][j] += beams_copy[a][b];
          N++;
        }
      }
      _light_beams[i][j] /= N;

    }
  }
}

void Camera::black_white()
{
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Black White", double(i)/_resolution_x);
    for(int j=100; j<_resolution_y; j++) {
      double avg = (_light_beams[i][j].red() + _light_beams[i][j].green() + _light_beams[i][j].blue()) / 3; 
      _light_beams[i][j] = LightBeam(avg, avg, avg);
    }
  }
}

void Camera::vignette()
{
  double diag = sqrt( pow(_resolution_x, 2) + pow(_resolution_y, 2) );
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Vignette", double(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {
      double offset_i = i - _resolution_x / 2.;
      double offset_j = j - _resolution_y / 2.;
      double relative_offset = sqrt( pow(offset_i, 2) + pow(offset_j, 2) ) / (diag/2);
      _light_beams[i][j] *= pow(2, _vignette * relative_offset);
    }
  }
}


int Camera::develop_beam(double l, double range_min, double range_max)
{
  double range_abs = range_max - range_min;
  double color;

  if(is_equal(range_abs, 0)) {
    color = 0;
  }
  else if(l <= range_min) {
    color = 0;
  }
  else {
    color = (l-range_min) / range_abs;
  }

  if(!is_equal(_noise, 0)) {
    color += f_rand(0, _noise/range_max);
  }

  if(color > 1) {
    color = 1;
  }

  return 255 * pow(color, _gamma);
}
