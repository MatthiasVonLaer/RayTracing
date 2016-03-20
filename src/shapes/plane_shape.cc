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

#include <iostream>
#include "plane_shape.h"
#include "utilities.h"

using namespace std;

PlaneShape::PlaneShape(Composition *parent) :
  Shape(parent),
  _image(0) 
{
  set_shape_type(SURFACE);
}

void PlaneShape::parse(const string &command, istream &in)
{
  if(command == "image") {
    string path;
    in >> path;
    load_image(path, _image);
  }
  else {
    Shape::parse(command, in);
  }
}

double PlaneShape::init_dx() {
  if(_image && height_parsed() && !width_parsed()) {
    return height() * _image->width() / _image->height();
  }
  else {
    return width();
  }
}

double PlaneShape::init_dz() {
  if(_image && !height_parsed() && width_parsed()) {
    return -width() * _image->height() / _image->width();
  }
  else {
    return -height();
  }
}

void PlaneShape::init_derived_class()
{
  _plane = Plane(origin(), vy());
}

bool PlaneShape::intersect(const Ray &ray, Plane &intersection_plane) const
{
  double distance;

  if(ray.intersect(_plane, distance)) {
    intersection_plane = Plane(ray.origin() + distance * ray.direction(), vy());
    return true;
  }
  else {
    return false;
  }
}

Color PlaneShape::get_color(const Vector &intersection_point) const
{
  if(color_type() == OPAQUE) {

    if(!_image) {
      return color();
    }

    else {
      Vector v = global_to_local_point(intersection_point);

      int x = (v.x() - floor(v.x())) * (_image->width()-1);
      int y = (v.z() - floor(v.z())) * (_image->height()-1);

      return _image->pixel(x, y);
    }
  }

  else {
    display_error("Unexpected Colortype.");
    return Color(255, 255, 255);
  }
}
