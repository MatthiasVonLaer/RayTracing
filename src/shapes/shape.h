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

class Composition;

class Shape
{
public:
  Shape(const Composition *parent);
  void init_unit_vectors();
  void init_transformation_matrix();
  virtual void init() {}
  virtual void parse(const std::string &command, std::istream &in);

public:
  virtual bool intersect(const Ray &ray, Plane &plane) const =0;
  virtual bool inside(const Ray &ray) const =0; 
  virtual Color get_color(const Vector &Vector) const =0;

protected:
  virtual double init_dx() = 0;
  virtual double init_dy() = 0;
  virtual double init_dz() = 0;
  virtual Vector init_origin() {return _center;}

public:
  ColorType     color_type() const        {return _color_type;}
  ShapeType     shape_type() const        {return _shape_type;}
  double        refraction_index() const  {return _refraction_index;}
  double        silvered() const          {return _silvered;}
  const Filter& filter() const            {return _filter;}
  bool          visible() const           {return _visible;}
  bool          regular() const           {return _regular;}

protected:
  void load_image(const std::string &path, QImage* &im) const;
  Vector global_to_local_point(const Vector &v) const;
  Vector global_to_local_direction(const Vector &v) const;
  Vector local_to_global_point(const Vector &v) const;
  Vector local_to_global_direction(const Vector &v) const;

  Vector        vx() const      {return _top_direction ^ _front_direction;}
  Vector        vy() const      {return -_front_direction;}
  Vector        vz() const      {return _top_direction;}
  double        dx() const      {return _dx;}
  double        dy() const      {return _dy;}
  double        dz() const      {return _dz;}
  double        width() const   {return _width;}
  double        height() const  {return _height;}
  double        depth() const   {return _depth;}
  const Vector& origin() const  {return _origin;}
  const Color&  color() const   {return _color;}

  void set_visibility(bool is)  {_visible = is;}

private:
  ColorType _color_type;
  ShapeType _shape_type;

  Color  _color;
  double _silvered;
  double _refraction_index;
  Filter _filter;

  bool   _visible;
  bool   _regular;

  Vector _center;
  Vector _origin;

  double _width;
  double _height;
  double _depth;

  double _dx; //right_left_direction
  double _dy; //front_direction
  double _dz; //top_direction

  Vector _top_direction;
  Vector _front_direction;
  bool _init_unit_vectors;

  Matrix _T_global_to_local;
  Matrix _T_local_to_global;
  bool _init_transformation_matrix;

  const Composition *_parent;
};
