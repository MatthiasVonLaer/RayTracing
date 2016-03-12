#pragma once

#include <fstream>
#include <vector>

#include <QImage>

#include "display.h"
#include "light_beam.h"
#include "lens.h"
#include "ray.h"
#include "scene.h"
#include "vector.h"

class Camera
{
public:
  Camera(Display &display);
  void parse(const std::string &command, std::istream &stream);
  void initialize();
  void take_picture(const Scene *scene);

private:
  void rename_previous_output() const;
  void shutter(const Scene *scene);
  void develop();

  void diffraction();
  void depth_of_field();
  void black_white();
  void vignette();
  double maximal_luminosity();
  int develop_beam(double l, double range_min, double range_max);

public:
  const Vector& position() const           {return _position;}
  const Vector& viewing_direction() const  {return _viewing_direction;}
  const Vector& top_direction() const      {return _top_direction;}
  int resolution_x() const                 {return _resolution_x;}
  int resolution_y() const                 {return _resolution_y;}

private:
  Display &_display;

  //Settings
  Vector _position;
  Vector _viewing_direction;
  Vector _top_direction;

  double _gamma;
  double _exposure_range;
  double _exposure;

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

  //Internal Components
  Lens _lens;
  QImage _film;

  std::vector < std::vector < LightBeam > > _light_beams;

  Vector _film_dx;
  Vector _film_dy;
  Vector _film_top_left_direction;

  int _shutter_pixel_x;
  int _shutter_pixel_y;
};
