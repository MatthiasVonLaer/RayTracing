//  Copyright 2016 Matthias von Laer
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "camera.h"

#include "complex_light_beam.h"
#include "mpi_manager.h"
#include "polygon.h"
#include "utilities.h"

#include <assert.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Camera::Camera(const Scene &scene, Display &display) :
  _scene(scene),
  _display(display),

  _initialized(false),

  _output_filename   ("raytracer.jpg"),
  _resolution_x      (750),
  _resolution_y      (500),

  _film_size_diagonal(sqrt(.036*.036 + .024*.024)), //full-frame film 36mm x 24mm
  _crop_factor       (1),

  _exposure_range    (-1),
  _exposure          (0),
  _gamma             (1),

  _cut_bright_lights (0),

  _depth_of_field    (false),
  _diffraction       (false),

  _colorization      ("rgb"),
  _vignette          (0),

  _position          (Vector()),
  _viewing_direction (Vector(0, 1, 0)),
  _top_direction     (Vector(0, 0, 1))
{
}

void Camera::parse(const string &command, istream &stream)
{
  _initialized = false;

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
  else if(command == "cut_bright_lights") {
    stream >> _cut_bright_lights;
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
  for(auto& row : _light_beams) {
    row.resize(_resolution_y);
  }

  //CALCULATE FILM ORIENTATION AND DIMENSION
  //For our Raytracing optic we imagine a film frame with a grid of resolution_x * resolution_y pixels.
  //We imagine that the film frame is placed the focal_length in front of the camera position.
  //The initial_rays are going from the camera position through the corresponding pixels on the frame.
  //In contrast to photography the film is placed in front of the camera instead of behind the lens,
  //however terms like film size or focal length are dealt in equal mesure.
  const double resolution_diagonal = sqrt(pow(_resolution_x-1, 2) + pow(_resolution_y-1, 2));
  const double film_size_x = (_resolution_x-1) / resolution_diagonal * _film_size_diagonal / _crop_factor;
  const double film_size_y = (_resolution_y-1) / resolution_diagonal * _film_size_diagonal / _crop_factor;

  const Vector right_direction = - (_top_direction ^ _viewing_direction).normalized();

  _film_top_left_direction  = _viewing_direction * _lens.focal_length();
  _film_top_left_direction -= right_direction * (film_size_x / 2);
  _film_top_left_direction += _top_direction * (film_size_y / 2);

  _film_dx = right_direction * (film_size_x / (_resolution_x-1));
  _film_dy = -_top_direction * (film_size_y / (_resolution_y-1));

  _initialized = true;
}


void Camera::take_picture()
{
  if(!_initialized) {
    display_error("Camera not initialized.");
  }

  _display.start_timer();

  shutter();
  develop();
  save();

  _display.stop_timer();
  _display.add_mpix(_resolution_x * _resolution_y);
}

void Camera::shutter()
{
  //serial-mode
  if(mpi().size() == 1) {
    for(int j=0; j<_resolution_y; j++) {
      for(int i=0; i<_resolution_x; i++) {

        Vector v = _film_top_left_direction + j*_film_dy + i*_film_dx;
        _light_beams[i][j] = _scene.raytracer(Ray(_position, v));

      }
      _display.progress("RayTracing", static_cast<double>(j)/_resolution_y);
    }
  }

  //paralell-mode
  else {
    for(int i=1; i<mpi().size(); i++) {
      //turn idle_sleep off for performance of slave-nodes
      mpi().send_order(MPI_Order::IDLE_SLEEP, i);
      mpi().send_int(false, i);

      //initialize slave-nodes
      mpi().send_order(MPI_Order::CAMERA_DATA, i);
      mpi().send_int(_resolution_x, i);
      mpi().send_vector(_position, i);
      mpi().send_vector(_film_top_left_direction, i);
      mpi().send_vector(_film_dx, i);
      mpi().send_vector(_film_dy, i);
    }

    //creating queue of waiting slave-nodes
    vector<int> queue;
    for(int i=1; i<mpi().size(); i++) {
      queue.push_back(i);
    }
    int sent = 0;
    int received = 0;
    while(true) {

      //as long as there are slaves in the waiting queue and there is work to be done
      while(queue.size() && sent < _resolution_y) {
        mpi().send_order(MPI_Order::RAYTRACE, queue[0]);
        mpi().send_int(sent, queue[0]);
        queue.erase(queue.begin());
        sent++;
        _display.progress("RayTracing", static_cast<double>(sent)/_resolution_y);
      }

      //break if all work is done
      if(received == _resolution_y) {
        break;
      }
      //wait for results
      else {
        int slave;
        mpi().recv_int(slave, MPI_ANY_SOURCE);
        int row;
        mpi().recv_int(row, slave);
        for(int i=0; i<_resolution_x; i++) {
          mpi().recv_light_beam(_light_beams[i][row], slave);
        }
        queue.push_back(slave);
        received++;
      }

    }
    //turn idle_sleep on to free cpu resources
    for(int i=1; i<mpi().size(); i++) {
      mpi().send_order(MPI_Order::IDLE_SLEEP, i);
      mpi().send_int(true, i);
    }
  }
}

void Camera::develop()
{
  if(_depth_of_field) {
    depth_of_field();
  }

  if(_diffraction) {
    diffraction();
  }

  if(_colorization == "bw") {
    black_white();
  }

  if(!is_equal(_vignette, 0)) {
    vignette();
  }


  double range_max;
  double range_min;
  calculate_exposure_range(range_min, range_max);


  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      _film.setPixel(i, j, qRgb(develop_beam(_light_beams[i][j].red()  , range_min, range_max),
                                develop_beam(_light_beams[i][j].green(), range_min, range_max),
                                develop_beam(_light_beams[i][j].blue() , range_min, range_max) ));
    }
    _display.progress("Develop", static_cast<double>(i)/_resolution_x);
  }
}

double Camera::calculate_exposure_range(double &range_min, double &range_max)
{
  //create histogram
  map<double, int> histogram;

  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      double lum = _light_beams[i][j].luminosity();
      if(histogram.count(lum)) {
        histogram[lum]++;
      }
      else {
        histogram[lum] = 1;
      }
    }
    _display.progress("Histogram", static_cast<double>(i)/_resolution_x);
  }

  //cut bright lights
  int n=0;
  double lum_max = 0;
  for(auto it = histogram.rbegin(); it != histogram.rend(); it++) {
    n += it->second;
    if(n > _cut_bright_lights * _resolution_x * _resolution_y) {
      lum_max = it->first;
      break;
    }
  }

  //calculate exposure range
  range_max = lum_max * pow(2, -_exposure);
  if(_exposure_range < 0)
    range_min = 0;
  else
    range_min = range_max * pow(2, - _exposure_range);
}

void Camera::depth_of_field()
{
  // blur_diameter / | distance - _focus |  =  (_focal_length/_aperture) / _focus
  // blur_diameter_on_film / blur_diameter  =  _focal_length / distance
  const vector< vector< LightBeam > > beams_copy = _light_beams;
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Depth of Field", static_cast<double>(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {
      double blur_diameter = _lens.blur_diameter( beams_copy[i][j].depth() );
      int blur_radius_pixel = blur_diameter / (_film_dx.norm()) / 2;
      if(blur_radius_pixel < 0) {
        stringstream stream;
        stream << "Blur_radius_pixel = " <<  blur_radius_pixel;
        display_warning(stream.str());
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

void Camera::diffraction()
{
  display_warning("Diffraction runs in experimental mode.");

  double range_max;
  double range_min;
  calculate_exposure_range(range_min, range_max);


  vector< vector< ComplexLightBeam > > diffracted_beams;
  diffracted_beams.resize(_resolution_x);
  for(int i=0; i<_resolution_x; i++) {
    diffracted_beams[i].resize(_resolution_y);
  }
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Diffraction", static_cast<double>(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {

      int length_max = 50;
      double ds = _film_dx.norm();

      LightBeam beam_root = _light_beams[i][j] ^ .5;
      diffracted_beams[i][j] += beam_root * _lens.diffraction_pattern(0, 0);

      for(int length=1; length < length_max; length++) {
        if(i-length>=0 && j-length>=0 && i+length<_resolution_x && j+length<_resolution_y) {
          bool visible = false;
          for(int s=-length; s<length; s++) {
            double radius = sqrt(length*length + s*s);
            double factor = (length_max - radius) / length_max;
            if(radius >= length_max) {
              continue;
            }
            diffracted_beams[i+s][j+length] += beam_root * (_lens.diffraction_pattern(s*ds, length*ds) * factor);
            diffracted_beams[i+s][j-length] += beam_root * (_lens.diffraction_pattern(s*ds, -length*ds) * factor);
            diffracted_beams[i+length][j+s] += beam_root * (_lens.diffraction_pattern(length*ds, s*ds) * factor);
            diffracted_beams[i-length][j+s] += beam_root * (_lens.diffraction_pattern(-length*ds, s*ds) * factor);
            if((beam_root * _lens.diffraction_pattern(s*ds, length*ds)).luminosity() > pow(range_min, .5)) {
              visible = true;
            }
          }
          if(!visible) {
            break;
          }
        }
        else {
          break;
        }
      }

    }
  }
  for(int i=0; i<_resolution_x; i++) {
    for(int j=0; j<_resolution_y; j++) {
      _light_beams[i][j] = abs_beam(diffracted_beams[i][j]) ^ 2;
    }
  }
}

void Camera::black_white()
{
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Black White", static_cast<double>(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {
      const double avg = (_light_beams[i][j].red() + _light_beams[i][j].green() + _light_beams[i][j].blue()) / 3; 
      _light_beams[i][j] = LightBeam(avg, avg, avg);
    }
  }
}

void Camera::vignette()
{
  const double diag = sqrt( pow(_resolution_x, 2) + pow(_resolution_y, 2) );
  for(int i=0; i<_resolution_x; i++) {
    _display.progress("Vignette", static_cast<double>(i)/_resolution_x);
    for(int j=0; j<_resolution_y; j++) {
      const double offset_i = i - _resolution_x / 2.;
      const double offset_j = j - _resolution_y / 2.;
      const double relative_offset = sqrt( pow(offset_i, 2) + pow(offset_j, 2) ) / (diag/2);
      _light_beams[i][j] *= pow(2, _vignette * relative_offset);
    }
  }
}


int Camera::develop_beam(double l, double range_min, double range_max)
{
  const double range_abs = range_max - range_min;
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

void Camera::save() const
{
  rename_previous_output();
  _film.save(_output_filename.c_str(), 0, 100);
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


Vector Camera::film_coordinates(Vector point) const
{
  const double z_distance = (point - _position) * _viewing_direction;
  if(z_distance < 0) {
    display_warning("z_dist < 0");
  }

  const double x = ((point - _position) * _film_dx.normalized()) * _lens.focal_length() /_film_dx.norm()/ z_distance ; 
  const double y = ((point - _position) * _film_dy.normalized()) * _lens.focal_length() /_film_dy.norm()/ z_distance ;

  return Vector(x + _resolution_x/2., y + _resolution_y/2., 0);
}
