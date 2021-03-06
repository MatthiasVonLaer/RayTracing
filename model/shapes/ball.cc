//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ball.h"

#include "utilities.h"

#include <iostream>

using namespace std;

Ball::Ball(Composition *parent):
  Shape(parent),
  _radius(1.)
{
}

void Ball::parse(const string &command, istream &in)
{
  if(command == "radius")
  {
    in >> _radius;
  }
  else if(command == "image")
  {
    string path;
    in >> path;

    _image.load(path.c_str());
    if(_image.isNull())
    {
      display_warning("Couldn't load image " + path);
    }
  }
  else
  {
    Shape::parse(command, in);
  }
}

std::optional<Plane> Ball::intersect(const Ray &ray) const
{
  // Solve:  (ray.origin + t * ray.direction - _origin)^2 = _radius^2
  // =>       t^2 + b*t + c = 0  with:
  double b = 2 * (ray.direction() * (ray.origin() - origin()));
  double c = (ray.origin() - origin()).norm2() - _radius*_radius;

  if(b*b-4*c < 0)
  {
    return std::nullopt;
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
      return Plane(p, n);
    }
    else if( is_greater(x2, 0) )
    {
      Vector p = ray.origin() + ray.direction() * x2;
      Vector n = p - origin();
      return Plane(p, n);
    }
    else
    {
      return std::nullopt;
    }
  }
}

bool Ball::inside(const Ray &ray) const
{
  Vector v = ray.origin() - origin();
  if (is_greater(1, v.norm()/_radius))
    return true;
  else if (is_equal(1, v.norm()/_radius) && is_greater(0, v * ray.direction()))
    return true;
  else
    return false;
}

Color Ball::get_color(const Vector &intersection_point) const
{
  if(color_type() == OPAQUE)
  {
    if(_image.isNull())
    {
      return color();
    }
    else
    {
      Vector v = global_to_local_point(intersection_point);

      double phi;
      if(v.x() <= 0)
        phi = acos( v.y() / sqrt(1-v.z()*v.z()));
      else
        phi = PI + acos( -v.y() / sqrt(1-v.z()*v.z()));

      double theta = acos( v.z() );

      return _image.pixel(_image.width() * phi/(2*PI),
                          _image.height() * theta/PI);
    }
  }
  else
  {
    display_error("Unexpected Colortype.");
    return Color(255, 255, 255);
  }
}
