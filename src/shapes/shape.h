#pragma once

#include <assert.h>

#include <QImage>

#include "color.h"
#include "filter.h"
#include "matrix.h"
#include "plane.h"
#include "ray.h"
#include "vector.h"

enum ColorType {OPAQUE, TRANSPARENT};
enum ShapeType {SOLID, SURFACE};

class Shape
{
public:
  Shape();
  void init_unit_vectors();
  void init_transformation_matrix();
  virtual void init() {}
  virtual void parse(const std::string &command, std::istream &in);

  virtual bool intersect(const Ray &ray, Plane &plane) const =0;
  virtual bool inside(const Ray &ray) const =0; 
  virtual Color get_color(const Vector &Vector) const =0;

  ColorType     color_type() const        {return _color_type;}
  ShapeType     shape_type() const        {return _shape_type;}
  double        refraction_index() const  {return _refraction_index;}
  double        silvered() const          {return _silvered;}
  const Filter& filter() const            {return _filter;}
  const Vector& position() const          {return _position;}
  bool          invisible() const         {return _invisible;}

protected:
  void load_image(const std::string &path, QImage* &im) const;
  Vector global_to_local_point(const Vector &v) const;
  Vector global_to_local_direction(const Vector &v) const;
  Vector local_to_global_point(const Vector &v) const;
  Vector local_to_global_direction(const Vector &v) const;
  Vector vx() const {assert(_init_unit_vectors); return _top_direction ^ _front_direction;}
  Vector vy() const {assert(_init_unit_vectors); return -_front_direction;}
  Vector vz() const {assert(_init_unit_vectors); return _top_direction;}

protected:
  ColorType _color_type;
  ShapeType _shape_type;

  Color  _color;
  double _silvered;
  double _refraction_index;
  Filter _filter;
  bool   _invisible;

  Vector _position;

  double _dx; //right_left_direction
  double _dy; //front_direction
  double _dz; //top_direction

private:
  Vector _top_direction;
  Vector _front_direction;
  bool _init_unit_vectors;

  Matrix _T_global_to_local;
  Matrix _T_local_to_global;
  bool _init_transformation_matrix;
};
