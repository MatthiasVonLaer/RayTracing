#include <iostream>

#include "cube.h"
#include "utilities.h"

using namespace std;

Cube::Cube(const Composition *parent) :
  Shape(parent)
{	
  for(int i=0; i<6; i++) {
    _image[i] = 0;
  }
}

Cube::~Cube()
{
  for(int i=0; i<6; i++ ) {
    if(_image[i]) {
      delete _image[i];
    }
  }
}

void Cube::parse(const string &command, istream &in)
{
  if(command == "image_all_sides") {
    string path;
    in >> path;
    load_image(path, _image[0]);
    load_image(path, _image[1]);
    load_image(path, _image[2]);
    load_image(path, _image[3]);
    load_image(path, _image[4]);
    load_image(path, _image[5]);
  }
  else if(command == "image") {
    int n;
    in >> n;
    string path;
    in >> path;
    load_image(path, _image[n]);
  }
  else {
    Shape::parse(command, in);
  }
}

void Cube::init_derived()
{
  _plane[0] = Plane( _position + _dx * vx(), vx());
  _plane[1] = Plane( _position + _dy * vy(), vy());
  _plane[2] = Plane( _position + _dz * vz(), vz());
  _plane[3] = Plane( _position - _dz * vz(), -vz());
  _plane[4] = Plane( _position - _dy * vy(), -vy());
  _plane[5] = Plane( _position - _dx * vx(), -vx());
}

bool Cube::intersect(const Ray &ray, Plane &intersection_plane) const
{
  int found_plane = -1;
  double dist_min;

  for(int i=0; i<6; i++)
  {
    double intersection_distance;
    if(ray.intersect(_plane[i], intersection_distance) &&
       (found_plane == -1 || is_greater(dist_min, intersection_distance))) {

      Vector point = global_to_local_point(ray.origin() + intersection_distance * ray.direction());
	
      if(is_greater_or_equal(1, fabs(point.x())) && is_greater_or_equal(1, fabs(point.y())) && is_greater_or_equal(1, fabs(point.z())) )
      {
        dist_min = intersection_distance;
        found_plane = i;
      }
    }
  }

  if(found_plane != -1) {
    intersection_plane = Plane(ray.origin() + dist_min * ray.direction(),
                               _plane[found_plane].normal());
    return true;
  }
  else {
    return false;
  }
}

bool Cube::inside(const Ray &ray) const
{
  Vector p = global_to_local_point(ray.origin());
  Vector u = global_to_local_direction(ray.direction());

  if(     is_greater(1, fabs(p.x())) && is_greater(1, fabs(p.y())) && is_greater(1, fabs(p.z())))
    return true;
  else if(is_equal(  1, p.x())       && is_greater(1, fabs(p.y())) && is_greater(1, fabs(p.z())) && is_greater(0, u.x()))
    return true;
  else if(is_equal( -1, p.x())       && is_greater(1, fabs(p.y())) && is_greater(1, fabs(p.z())) && is_greater(u.x(), 0))
    return true;
  else if(is_equal(  1, p.y())       && is_greater(1, fabs(p.x())) && is_greater(1, fabs(p.z())) && is_greater(0, u.y()))
    return true;
  else if(is_equal( -1, p.y())       && is_greater(1, fabs(p.x())) && is_greater(1, fabs(p.z())) && is_greater(u.y(), 0))
    return true;
  else if(is_equal(  1, p.z())       && is_greater(1, fabs(p.x())) && is_greater(1, fabs(p.y())) && is_greater(0, u.z()))
    return true;
  else if(is_equal( -1, p.z())       && is_greater(1, fabs(p.x())) && is_greater(1, fabs(p.y())) && is_greater(u.z(), 0))
    return true;
  else
    return false;
}

Color Cube::get_color(const Vector &intersection_point) const
{
  if(_color_type == OPAQUE) {

    Vector v = global_to_local_point(intersection_point);
    double a, b;
    int i = -1;
    if(is_equal(v.x(), 1))      { i = 0; a = v.y(); b = v.z(); }
    else if(is_equal(v.x(),-1)) { i = 5; a =-v.y(); b = v.z(); }
    else if(is_equal(v.y(), 1)) { i = 1; a =-v.x(); b = v.z(); }
    else if(is_equal(v.y(),-1)) { i = 4; a = v.x(); b = v.z(); }
    else if(is_equal(v.z(), 1)) { i = 2; a = v.x(); b = v.y(); }
    else if(is_equal(v.z(),-1)) { i = 3; a =-v.x(); b = v.y(); }

    if(i == -1 || !_image[i]) {
      return _color;
    }
    else {
      return _image[i]->pixel((a+1)/2 * (_image[i]->width()-1), (b+1)/2 * (_image[i]->height()-1));
    }
  }

  else {
    display_error("Unexpected Colortype.");
    return Color(255, 255, 255);
  }
}
