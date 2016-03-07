#include <iostream>

#include "ball.h"
#include "utilities.h"

using namespace std;

Ball::Ball():
  Shape(),
  _radius(0),
  _image(0)
{
}

Ball::~Ball()
{
  if(_image)
    delete _image;
}

void Ball::init()
{
  _dx = _dy = _dz = _radius;
}

void Ball::parse(const string &command, istream &in)
{
  if(command == "radius") {
    in >> _radius;
  }
  else if(command == "image") {
    string path;
    in >> path;
    load_image(path, _image);
  }
  else {
    Shape::parse(command, in);
  }
}

bool Ball::intersect(const Ray &ray, Plane &plane) const
{
  // Solve:  (ray.origin + t * ray.direction - _position)^2 = _radius^2
  // =>       t^2 + b*t + c = 0  with:
  double b = 2 * (ray.direction() * (ray.origin() - _position));
  double c = (ray.origin() - _position).norm2() - _radius*_radius;

  if(b*b-4*c < 0) {
    return false;
  }
  else
  {
    double w = sqrt(b*b - 4*c);
    double x1 = (-w - b)/2.;
    double x2 = (w - b)/2.;
    if( is_greater(x1, 0) ) 
    {
      Vector p = ray.origin() + ray.direction() * x1;
      Vector n = p - _position;
      plane = Plane(p, n);
      return true;
    }
    else if( is_greater(x2, 0) )
    {
      Vector p = ray.origin() + ray.direction() * x2;
      Vector n = p - _position;
      plane = Plane(p, n);
      return true;
    }
    else {
      return false;
    }
  }
}

bool Ball::inside(const Ray &ray) const
{
  Vector v = ray.origin() - _position;
  if(   is_greater(_radius, v.norm()))
    return true;
  else if(is_equal(_radius, v.norm()) && is_greater(0, v * ray.direction()))
    return true;
  else
    return false;
}

Color Ball::get_color(const Vector &intersection_point) const
{
  if(_color_type == OPAQUE) {

    if(!_image) {
      return _color;
    }

    else {
      Vector v = global_to_local_point(intersection_point);

      double phi;
      if(v.x() <= 0)
        phi = acos( v.y() / sqrt(1-v.z()*v.z()));
      else
        phi = PI + acos( -v.y() / sqrt(1-v.z()*v.z()));

      double theta = acos( v.z() );

      return _image->pixel(_image->width() * phi/(2*PI),
                           _image->height() * theta/PI);
    }
  }

  else {
    display_error("Unexpected Colortype.");
    return Color(255, 255, 255);
  }
}
