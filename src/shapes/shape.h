//  Copyright 2016 Matthias von Laer
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <assert.h>

#include <QImage>

#include "color.h"
#include "filter.h"
#include "matrix.h"
#include "plane.h"
#include "ray.h"
#include "vector.h"

enum ShapeType {SOLID, SURFACE};
enum ColorType {OPAQUE, TRANSPARENT};

class Composition;

class Shape
{
private:
  Composition *_parent;

  ShapeType _shape_type;
  ColorType _color_type;

  Vector _origin;
  Vector _top_direction;
  Vector _front_direction;

  Color  _color;
  double _silvered;
  double _refraction_index;
  Filter _filter;

  bool   _visible;
  bool   _regular;

  //local unit vectors
  Vector _vx;
  Vector _vy;
  Vector _vz;

  //dimensions of local coordinates
  double _dx; //right_left_direction
  double _dy; //front_direction
  double _dz; //top_direction

  //transformation matrices
  Matrix _T_global_to_local;
  Matrix _T_local_to_global;

  //parsed variables to be processed in the init sequence
  double _width, _depth, _height;
  bool _width_parsed, _depth_parsed, _height_parsed;
  Vector _position;
  

public:
  Shape(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in);

public:
  void init_sequence();
private:
  virtual double init_dx()          {return _width/2;}
  virtual double init_dy()          {return _depth/2;}
  virtual double init_dz()          {return _height/2;}
  void init_local_coordinates();
  virtual Vector init_origin()      {return _position;}
  virtual void init_derived_class() {}

public:
  virtual bool intersect(const Ray &ray, Plane &plane)  const =0;
  virtual bool inside(const Ray &ray)                   const =0; 
  virtual Color get_color(const Vector &Vector)         const =0;


protected:
  Vector global_to_local_point(const Vector &v) const;
  Vector global_to_local_direction(const Vector &v) const;
  Vector local_to_global_point(const Vector &v) const;
  Vector local_to_global_direction(const Vector &v) const;

  const Vector& vx() const {return _vx;}
  const Vector& vy() const {return _vy;}
  const Vector& vz() const {return _vz;}

  const Matrix& transformation_matrix() const {return _T_local_to_global;}

  const Color&  color() const             {return _color;}

  const Vector& position() const {return _position;}
  double width() const        {return _width;}
  double depth() const        {return _depth;}
  double height() const       {return _height;}
  bool width_parsed() const   {return _width_parsed;}
  bool depth_parsed() const   {return _depth_parsed;}
  bool height_parsed() const  {return _height_parsed;}

  void set_visible(bool is)           {_visible = is;}
  void set_shape_type(ShapeType type) {_shape_type = type;}

  void load_image(const std::string &path, QImage* &im) const;

public:
  const Vector& origin() const            {return _origin;}
  ColorType     color_type() const        {return _color_type;}
  ShapeType     shape_type() const        {return _shape_type;}
  double        refraction_index() const  {return _refraction_index;}
  double        silvered() const          {return _silvered;}
  const Filter& filter() const            {return _filter;}
  bool          visible() const           {return _visible;}
  bool          regular() const           {return _regular;}
};
