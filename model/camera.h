//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "display.h"
#include "light_beam.h"
#include "lens.h"
#include "ray.h"
#include "scene.h"
#include "vector.h"

#include <QImage>

#include <fstream>
#include <vector>

class Camera
{
private:
  //the member _scene is only used in serial-mode.
  //In parallel mode camera calls slaves via mpi
  //and the slaves control the scene
  const Scene &_scene;

  Display &_display;
  Lens _lens;
  QImage _film;

  std::vector < std::vector < LightBeam > > _light_beams;

  Vector _film_dx;
  Vector _film_dy;
  Vector _film_top_left_direction;

  bool _initialized;

  //Settings
  Vector _position;
  Vector _viewing_direction;
  Vector _top_direction;

  double _gamma;
  double _exposure_range;
  double _exposure;

  double _cut_bright_lights;

  bool _depth_of_field;
  bool _diffraction;

  std::string _colorization;
  double _vignette;
  double _noise;

  int _resolution_x;
  int _resolution_y;
  double _crop_factor;
  double _film_size_diagonal;

  std::string _output_filename;

public:
  Camera(const Scene &scene, Display &display);
  void parse(const std::string &command, std::istream &stream);
  void initialize();
  void take_picture();

private:
  void shutter();
  void develop();

  void calculate_exposure_range(double &range_min, double &range_max);
  void depth_of_field();
  void diffraction();
  void black_white();
  void vignette();
  int develop_beam(double l, double range_min, double range_max);

public:
  void save() const;
private:
  void rename_previous_output() const;

public:
  Vector film_coordinates(Vector point) const;

public:
  const Vector& position() const           {return _position;}
  const Vector& viewing_direction() const  {return _viewing_direction;}
  const Vector& top_direction() const      {return _top_direction;}
  int resolution_x() const                 {return _resolution_x;}
  int resolution_y() const                 {return _resolution_y;}
  const Vector& film_top_left_direction() const {return _film_top_left_direction;}
  const Vector& film_dx() const            {return _film_dx;}
  const Vector& film_dy() const            {return _film_dy;} 
  const QImage& film() const               {return _film;}
  double exposure() const                  {return _exposure;}
  double focal_length() const              {return _lens.focal_length();}

  void set_position(const Vector &v)          {_position = v;}
  void set_viewing_direction(const Vector &v) {_initialized = false; _viewing_direction = v;}
  void set_top_direction(const Vector &v)     {_initialized = false; _top_direction = v;}
  void set_resolution(int x, int y)           {_initialized = false; _resolution_x = x; _resolution_y = y;}
  void set_exposure(double d)                 {_exposure = d;}
  void set_focal_length(double d)             {_lens.set_focal_length(d);}
};
