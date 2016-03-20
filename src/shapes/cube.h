#pragma once

#include <vector>

#include "plane.h"
#include "shape.h"
#include "vector.h"

class Cube : public Shape
{
private:
  Plane _plane[6];
  QImage *_image[6];

public:
  Cube(Composition *parent);
  ~Cube();
  virtual void parse(const std::string &command, std::istream &in);

private:
  void init_derived_class();
	
public:
  bool intersect(const Ray &ray, Plane &intersection_plane) const;
  bool inside(const Ray &ray) const;
  Color get_color(const Vector &intersection_point) const;
};
