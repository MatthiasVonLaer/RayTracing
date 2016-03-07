#pragma once

#include <vector>

#include "plane.h"
#include "shape.h"
#include "vector.h"

class Cube : public Shape
{
public:
  Cube();
  ~Cube();
  void parse(const std::string &command, std::istream &in);
  void init();
	
  bool intersect(const Ray &ray, Plane &intersection_plane) const;
  bool inside(const Ray &ray) const;
  Color get_color(const Vector &intersection_point) const;
	
private:
  Plane _plane[6];
  QImage *_image[6];
};
