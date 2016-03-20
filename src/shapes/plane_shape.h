#pragma once

#include "plane.h"
#include "shape.h"

class PlaneShape : public Shape
{
private:
  Plane _plane;
  QImage *_image;

public:
  PlaneShape(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in);

private:
  double init_dx();
  double init_dz();
  void init_derived_class();

public:
  bool intersect(const Ray &ray, Plane &intersection_plane) const;
  bool inside(const Ray&) const {return false;}
  Color get_color(const Vector &intersection_point) const;
};
