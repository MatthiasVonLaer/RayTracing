#pragma once

#include "plane.h"
#include "shape.h"

class PlaneShape : public Shape
{
 public:
  PlaneShape();

  void init();
  void parse(const std::string &command, std::istream &in);

  bool intersect(const Ray &ray, Plane &intersection_plane) const;
  bool inside(const Ray&) const {return false;}
  Color get_color(const Vector &intersection_point) const;

 private:
  Plane _plane;
  QImage *_image;
  double _im_width;
  double _im_height;
};
