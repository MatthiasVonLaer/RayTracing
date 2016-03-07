#pragma once

#include "color.h"
#include "filter.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

class Ball : public Shape
{
public:
  Ball();
  ~Ball();

  void init();
  void parse(const std::string &command, std::istream &in);

  bool intersect(const Ray &ray, Plane &plane) const;
  bool inside(const Ray &ray) const;
  Color get_color(const Vector &intersection_point) const;

protected:
  double _radius;
  QImage *_image;
};
