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

#include "cube.h"

#include "utilities.h"

#include <iostream>

using namespace std;

Cube::Cube(Composition *parent) :
  Shape(parent)
{	
  for(int i=0; i<6; i++) {
    _image[i] = 0;
  }
}

void Cube::parse(const string &command, istream &in)
{
  if(command == "image_all_sides") {
    string path;
    in >> path;

    auto image_all_sides = make_shared<QImage>(path.c_str());

    if(image_all_sides->isNull()) {
      display_warning("Couldn't load image " + path);
    }
    else {
      _image[0] = image_all_sides;
      _image[1] = image_all_sides;
      _image[2] = image_all_sides;
      _image[3] = image_all_sides;
      _image[4] = image_all_sides;
      _image[5] = image_all_sides;
    }
  }
  else if(command == "image") {
    int n;
    in >> n;

    string path;
    in >> path;

    _image[n] = make_shared<QImage>(path.c_str());

    if(_image[n]->isNull()) {
      display_warning("Couldn't load image " + path);
    }
  }
  else {
    Shape::parse(command, in);
  }
}

void Cube::init_derived_class()
{
  _plane[0] = Plane( origin() + vx(), vx());
  _plane[1] = Plane( origin() + vy(), vy());
  _plane[2] = Plane( origin() + vz(), vz());
  _plane[3] = Plane( origin() - vz(), -vz());
  _plane[4] = Plane( origin() - vy(), -vy());
  _plane[5] = Plane( origin() - vx(), -vx());
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
  if(color_type() == OPAQUE) {

    Vector v = global_to_local_point(intersection_point);
    double a, b;
    int i = -1;
    if(is_equal(v.x(), 1))      { i = 0; a = v.y(); b = v.z(); }
    else if(is_equal(v.x(),-1)) { i = 5; a =-v.y(); b = v.z(); }
    else if(is_equal(v.y(), 1)) { i = 1; a =-v.x(); b = v.z(); }
    else if(is_equal(v.y(),-1)) { i = 4; a = v.x(); b = v.z(); }
    else if(is_equal(v.z(), 1)) { i = 2; a = v.x(); b = v.y(); }
    else if(is_equal(v.z(),-1)) { i = 3; a =-v.x(); b = v.y(); }
    else                        { i = 0; a = v.y(); b = v.z(); }

    if(!_image[i] || _image[i]->isNull()) {
      return color();
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
