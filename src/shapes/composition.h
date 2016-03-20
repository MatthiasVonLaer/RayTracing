#pragma once

#include <fstream>
#include <string>

#include "shape.h"
#include "utilities.h"

class Scene;

class Composition : public Shape
{
private:
  const Composition *_parent;
  Scene *_scene;

  double _scale;

  std::string _filename;
  std::string _path;
  std::ifstream _input;

public:
  Composition(Composition *parent);
  virtual void parse(const std::string &command, std::istream &in);
  void setup(Scene *scene, const Composition *parent, const std::string &path);

  double init_dx()          {return _scale;}
  double init_dy()          {return _scale;}
  double init_dz()          {return _scale;}

  const Matrix& transformation_matrix() const {return Shape::transformation_matrix();}

  bool intersect(const Ray &ray, Plane &plane) const  {display_error("Composition should be invisible");}
  bool inside(const Ray &ray) const                   {display_error("Composition should be invisible");}
  Color get_color(const Vector &Vector) const         {display_error("Composition should be invisible");}

  std::string name() const;
};
