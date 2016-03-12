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

  virtual bool intersect(const Ray &ray, Plane &plane) const;
  virtual bool inside(const Ray &ray) const;
  virtual Color get_color(const Vector &intersection_point) const;

  virtual double init_dx();
  virtual double init_dy();
  virtual double init_dz();

protected:
  double _radius;
  QImage *_image;
};
