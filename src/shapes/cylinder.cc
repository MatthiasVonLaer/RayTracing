#include <iostream>

#include "cylinder.h"
#include "utilities.h"

using namespace std;

Cylinder::Cylinder(Composition *parent) :
  Shape(parent),
  _radius(0)
{	
}

Cylinder::~Cylinder()
{
}

void Cylinder::parse(const string &command, istream &in)
{
  if(command == "radius") {
    in >> _radius;
  }
  else {
    Shape::parse(command, in);
  }
}

void Cylinder::init_derived_class()
{
  _plane[0] = Plane( origin() + vz(), vz());
  _plane[1] = Plane( origin() - vz(), -vz());
}

bool Cylinder::intersect(const Ray &ray, Plane &intersection_plane) const
{
  Plane circle_intersection;
  Plane plane_intersection;
  bool is_circle = intersect_circle(ray, circle_intersection);
  bool is_plane = intersect_plane(ray, plane_intersection);

  if(is_circle && is_plane) {
    if((ray.origin() - circle_intersection.origin()).norm() < (ray.origin() - plane_intersection.origin()).norm()) {
      intersection_plane = circle_intersection;
    }
    else {
      intersection_plane = plane_intersection;
    }
    return true;
  }
  else if(is_circle) {
    intersection_plane = circle_intersection;
    return true;
  }
  else if(is_plane) {
    intersection_plane = plane_intersection;
    return true;
  }
  else {
    return false;
  }
}

bool Cylinder::intersect_plane(const Ray &ray, Plane &intersection_plane) const
{
  int found_plane = -1;
  double dist_min;

  for(int i=0; i<2; i++)
  {
    double intersection_distance;
    if(ray.intersect(_plane[i], intersection_distance) &&
       (found_plane == -1 || is_greater(dist_min, intersection_distance))) {

      Vector point = global_to_local_point(ray.origin() + intersection_distance * ray.direction());
	
      if(is_greater_or_equal(1, sqrt(pow(point.x(), 2) + pow(point.y(), 2))))
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

bool Cylinder::intersect_circle(const Ray &ray, Plane &plane) const
{
  Vector rp = global_to_local_point(ray.origin());
  Vector ru = global_to_local_direction(ray.direction());

  Vector rp_proj = rp;
  Vector ru_proj = ru;

  rp_proj.set_z(0);
  ru_proj.set_z(0);

  // Solve:  (ray.origin + t * ray.direction - _origin)^2 = _radius^2
  // =>       t^2 + b*t + c = 0  with:
  double a = ru_proj.norm2();
  double b = 2 * (ru_proj * rp_proj);
  double c = rp_proj.norm2() - 1;

  if(b*b-4*c < 0) {
    return false;
  }
  else
  {
    double w = sqrt(b*b - 4*a*c);
    double x1 = (-w - b)/(2.*a);
    double x2 = (w - b)/(2.*a);
    if( is_greater(x1, 0) ) 
    {
      Vector p = ray.origin() + ray.direction() * x1;
      Vector n = local_to_global_direction(rp_proj + ru_proj * x1);
      plane = Plane(p, n);
      return is_smaller( fabs(rp.z() + ru.z() * x1), 1 );
    }
    else if( is_greater(x2, 0) )
    {
      Vector p = ray.origin() + ray.direction() * x2;
      Vector n = local_to_global_direction(rp_proj + ru_proj * x2);
      plane = Plane(p, n);
      return is_smaller( fabs(rp.z() + ru.z() * x2), 1 );
    }
    else {
      return false;
    }
  }
}

bool Cylinder::inside(const Ray &ray) const
{
  Vector p = global_to_local_point(ray.origin());
  Vector u = global_to_local_direction(ray.direction());
  double r = sqrt(pow(p.x(), 2) + pow(p.y(), 2));

  if(     is_greater(1, r) && is_greater(1, fabs(p.z())))
    return true;
  else if(is_equal(  1, r)           && is_greater(1, fabs(p.z())) && is_greater(0, u.x()*p.x() + u.y()*p.y()))
    return true;
  else if(is_equal(  1, p.z())       && is_greater(1, r) && is_greater(0, u.z()))
    return true;
  else if(is_equal( -1, p.z())       && is_greater(1, r) && is_greater(u.z(), 0))
    return true;
  else
    return false;
}

Color Cylinder::get_color(const Vector &intersection_point) const
{
  if(color_type() == OPAQUE) {
    return color();
  }

  else {
    display_error("Unexpected Colortype.");
    return Color(255, 255, 255);
  }
}
