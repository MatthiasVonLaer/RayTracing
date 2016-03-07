#pragma once

#include <fstream>
#include <string>
#include <QImage>
#include <QPainter>

#include "matrix.h"
#include "ray.h"
#include "vector.h"

class RayDiagram
{
public:
  RayDiagram();
  ~RayDiagram();
  void parse(const std::string &line);
  void init(const Vector &position, const Vector &viewing_direction, const Vector &top_direction, int res_x, int res_y);

  void set_pixel_in_progress(int x, int y);
  void add(const Ray &ray, double distance, double ratio);

  void save() const;
  bool enabled() const  {return _enabled;}

private:
  Vector _position;
  Matrix _transformation_matrix;
  double _range;
  int _width;
  int _height;
  int _y0;
  int _number_rays;
  int _resolution_x;

  std::string _output_file;

  bool _enabled;
  int _pixel_in_progress_x;
  int _pixel_in_progress_y;

  QImage _image;
  QPainter *_painter;
};