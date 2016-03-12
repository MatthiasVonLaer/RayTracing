#pragma once

#include "plane.h"
#include "shape.h"

class PlaneShape : public Shape
{
 public:
  PlaneShape(const Composition *parent);

  virtual void parse(const std::string &command, std::istream &in);

 protected:
  virtual void init_derived();

 public:
  virtual bool intersect(const Ray &ray, Plane &intersection_plane) const;
  virtual bool inside(const Ray&) const {return false;}
  virtual Color get_color(const Vector &intersection_point) const;

 private:
  Plane _plane;
  QImage *_image;
  double _im_width;
  double _im_height;
};
