#include <iostream>

#include "ball.h"
#include "utilities.h"

using namespace std;

Ball::Ball(Composition *parent):
  Shape(parent),
  _radius(1.),
  _image(0)
{
}

Ball::~Ball()
{
  delete _image;
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
  // Solve:  (ray.origin + t * ray.direction - _origin)^2 = _radius^2
  // =>       t^2 + b*t + c = 0  with:
  double b = 2 * (ray.direction() * (ray.origin() - origin()));
  double c = (ray.origin() - origin()).norm2() - _radius*_radius;

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
      Vector n = p - origin();
      plane = Plane(p, n);
      return true;
    }
    else if( is_greater(x2, 0) )
    {
      Vector p = ray.origin() + ray.direction() * x2;
      Vector n = p - origin();
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
  Vector v = ray.origin() - origin();
  if(   is_greater(1, v.norm()/_radius))
    return true;
  else if(is_equal(1, v.norm()/_radius) && is_greater(0, v * ray.direction()))
    return true;
  else
    return false;
}

Color Ball::get_color(const Vector &intersection_point) const
{
  if(color_type() == OPAQUE) {

    if(!_image) {
      return color();
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
