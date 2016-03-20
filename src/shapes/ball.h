#pragma once

#include "color.h"
#include "filter.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

class Ball : public Shape
{
private:
  double _radius;
  QImage *_image;

public:
  Ball(Composition *parent);
  ~Ball();

  virtual void parse(const std::string &command, std::istream &in);

private:
  double init_dx() {return _radius;}
  double init_dy() {return _radius;}
  double init_dz() {return _radius;}

public:
  bool intersect(const Ray &ray, Plane &plane) const;
  bool inside(const Ray &ray) const;
  Color get_color(const Vector &intersection_point) const;

protected:
  void set_radius(double r) {_radius = r;}
};
