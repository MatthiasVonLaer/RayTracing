#pragma once

#include <vector>

#include "plane.h"
#include "shape.h"
#include "vector.h"

class Cube : public Shape
{
public:
  Cube(const Composition *parent);
  ~Cube();
  virtual void parse(const std::string &command, std::istream &in);
  virtual void init();
	
  virtual bool intersect(const Ray &ray, Plane &intersection_plane) const;
  virtual bool inside(const Ray &ray) const;
  virtual Color get_color(const Vector &intersection_point) const;
	
private:
  Plane _plane[6];
  QImage *_image[6];
};
