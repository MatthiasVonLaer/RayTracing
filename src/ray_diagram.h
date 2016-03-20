#pragma once

#include <fstream>
#include <string>
#include <QImage>
#include <QPainter>

#include "camera.h"
#include "matrix.h"
#include "ray.h"
#include "scene.h"
#include "shape.h"
#include "vector.h"

class RayDiagram
{
private:
  static QColor color_of_shape(const Shape* shape);

private:
  const Scene &_scene;
  const Camera &_camera;

  QImage _image;
  QPainter *_painter;

  Matrix _transformation_matrix;

  double _range;
  int _width;
  int _height;
  int _y0;
  int _number_rays;

  std::string _output_file;

  bool _enabled;

public:
  RayDiagram(const Scene &scene, const Camera &camera);
  ~RayDiagram();

  void parse(const std::string &command, std::istream &stream);
  void init();

public:
  void track();
private:
  void paint_ray(const Ray &ray, double distance, double ratio);
  void paint_shape(const Ray &ray, const Shape *shape, double distance);

public:
  void save() const;

  bool enabled() const  {return _enabled;}
};
