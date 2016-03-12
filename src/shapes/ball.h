#pragma once

#include "color.h"
#include "filter.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

class Ball : public Shape
{
public:
  Ball(const Composition *parent);
  ~Ball();

  virtual void parse(const std::string &command, std::istream &in);

protected:
  void init_derived();

public:
  virtual bool intersect(const Ray &ray, Plane &plane) const;
  virtual bool inside(const Ray &ray) const;
  virtual Color get_color(const Vector &intersection_point) const;

protected:
  double _radius;
  QImage *_image;
};
