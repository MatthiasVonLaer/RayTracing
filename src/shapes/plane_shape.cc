#include <iostream>
#include "plane_shape.h"
#include "utilities.h"

using namespace std;

PlaneShape::PlaneShape() :
  _image(0),
  _im_width(0),
  _im_height(0)
{
  _shape_type = SURFACE;
}

void PlaneShape::init()
{
  _plane = Plane(_position, vy());

  if(_image) {
    if(!is_equal(0, _im_height) &&
       !is_equal(0, _im_width))
    {
      _dx = _im_width;
      _dz = -_im_height;
    }
    else if(!is_equal(0, _im_width) &&
             is_equal(0, _im_height))
    {
      _dx = _im_width;
      _dz = -_im_width * _image->height() / _image->width();
    }
    else if(!is_equal(0, _im_height) &&
             is_equal(0, _im_width))
    {
      _dz = -_im_height;
      _dx = _im_height * _image->width() / _image->height();
    }
    else {
      _dz = -1;
    }
  }
}

void PlaneShape::parse(const string &command, istream &in)
{
  if(command == "height") {
    in >> _im_height;
  }
  else if(command == "image") {
    string path;
    in >> path;
    load_image(path, _image);
  }
  else if(command == "width") {
    in >> _im_width;
  }
  else {
    Shape::parse(command, in);
  }
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
  if(_color_type == OPAQUE) {

    if(!_image) {
      return _color;
    }

    else {
      Vector v = global_to_local_point(intersection_point);

      int x = (v.x() - round_down(v.x())) * (_image->width()-1);
      int y = (v.z() - round_down(v.z())) * (_image->height()-1);

      return _image->pixel(x, y);
    }
  }

  else {
    display_error("Unexpected Colortype.");
    return Color(255, 255, 255);
  }
}
