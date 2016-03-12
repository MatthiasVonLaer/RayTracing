#pragma once

#include <fstream>
#include <string>

#include "shape.h"

class Composition : public Shape
{
public:
  Composition(const Composition *parent);
  virtual void parse(const std::string &command, std::istream &in);

  virtual bool intersect(const Ray &ray, Plane &plane) const {}
  virtual bool inside(const Ray &ray) const {}
  virtual Color get_color(const Vector &Vector) const {}

private:
  std::string _path;
  std::string _name;
  std::ifstream _input;
};
