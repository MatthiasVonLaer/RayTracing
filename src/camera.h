#pragma once

#include <fstream>
#include <vector>

#include <QImage>

#include "light_beam.h"
#include "ray.h"
#include "ray_diagram.h"
#include "scene.h"
#include "vector.h"

class Camera
{
public:
  Camera();
  void parse(std::istream &in);
private:
  void take_picture();
  void rename_previous_output() const;
  void initialize();
  void shutter();
  void develop();
  int develop_beam(double l);

  void display_summary();
  void display_progress(const std::string &name, double progress);

private:
  //Settings
  Vector _position;
  Vector _viewing_direction;
  Vector _top_direction;

  double _gamma;
  double _exposure_range;
  double _exposure;

  double _aperture;
  double _focus;
  bool _depth_of_field;

  std::string _colorization;
  double _vignette;
  double _noise;

  int _resolution_x;
  int _resolution_y;
  double _focal_length;
  double _crop_factor;
  double _film_size_diagonal;

  std::string _output_filename;

  //Internal Components
  std::vector < std::vector < LightBeam > > _light_beams;
  Scene _scene;
  QImage _film;

  Vector _film_dx;
  Vector _film_dy;
  Vector _film_top_left_direction;

  double _range_max;
  double _range_min;
  double _range_abs;

  double _summary_time;
  double _summary_mpix;
  int _summary_pics;

  bool _display_progress;
  bool _display_summary;

  RayDiagram _ray_diagram;
};
