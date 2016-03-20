#pragma once

#include <vector>

#include "plane.h"
#include "shape.h"
#include "vector.h"

class Cylinder : public Shape
{
private:
  Plane _plane[2];
  double _radius;

public:
  Cylinder(Composition *parent);
  ~Cylinder();
  virtual void parse(const std::string &command, std::istream &in);

private:
  double init_dx() {return _radius;}
  double init_dy() {return _radius;}
  void init_derived_class();
	
public:
  bool intersect(const Ray &ray, Plane &intersection_plane) const;
private:
  bool intersect_plane(const Ray &ray, Plane &intersection_plane) const;
  bool intersect_circle(const Ray &ray, Plane &intersection_plane) const;
public:
  bool inside(const Ray &ray) const;
  Color get_color(const Vector &intersection_point) const;
};
