//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "function_plot.h"

#include "utilities.h"

#include <iostream>
#include <vector>

using namespace std;

FunctionPlot::FunctionPlot(Composition *parent) :
  Shape(parent),
  _xmin(-1),
  _ymin(-1),
  _zmin(-1),
  _xmax(1),
  _ymax(1),
  _zmax(1)
{	
  set_shape_type(SURFACE);
}

void FunctionPlot::parse(const string &command, istream &in)
{
  if(command == "function_0") {
    string expr;
    getline(in, expr);
    _f0 = expr;
    _f0_x = _f0.dx();
    _f0_y = _f0.dy();
  }
  else if(command == "function_1") {
    set_shape_type(SOLID);
    string expr;
    getline(in, expr);
    _f1 = expr;
    _f1_x = _f1.dx();
    _f1_y = _f1.dy();
  }
  else if(command == "xrange") {
    in >> _xmin >> _xmax;
  }
  else if(command == "yrange") {
    in >> _ymin >> _ymax;
  }
  else if(command == "zrange") {
    in >> _zmin >> _zmax;
  }
  else {
    Shape::parse(command, in);
  }
}

Vector FunctionPlot::init_origin()
{
  Vector origin(position());
  origin -= ((_xmax+_xmin)/2) * vx();
  origin -= ((_ymax+_ymin)/2) * vy();
  origin -= ((_zmax+_zmin)/2) * vz();
  return origin;
}

void FunctionPlot::init_derived_class()
{
  if(is_greater_or_equal(_xmin, _xmax) ||is_greater_or_equal(_ymin, _ymax) ||  is_greater_or_equal(_zmin, _zmax)) {
    display_error("FunctionPlot: min >= max");
  }

  _plane[PLANE_XMAX] = Plane( origin() + _xmax * vx(), vx());
  _plane[PLANE_YMAX] = Plane( origin() + _ymax * vy(), vy());
  _plane[PLANE_ZMAX] = Plane( origin() + _zmax * vz(), vz());
  _plane[PLANE_ZMIN] = Plane( origin() + _zmin * vz(), -vz());
  _plane[PLANE_YMIN] = Plane( origin() + _ymin * vy(), -vy());
  _plane[PLANE_XMIN] = Plane( origin() + _xmin * vx(), -vx());
}

std::optional<Plane> FunctionPlot::intersect(const Ray &ray) const
{
  double dist[2];
  int intersections_found = 0;

  for(int i=0; i<6; i++)
  {
    double intersection_distance;
    if(ray.intersect(_plane[i], intersection_distance))
    {
      Vector point = global_to_local_point(ray.origin() + intersection_distance * ray.direction());
	
      if(is_greater_or_equal(_xmax, point.x()) && is_greater_or_equal(_ymax, point.y()) && is_greater_or_equal(_zmax, point.z()) &&
         is_smaller_or_equal(_xmin, point.x()) && is_smaller_or_equal(_ymin, point.y()) && is_smaller_or_equal(_zmin, point.z()) )
      {
        dist[intersections_found] = intersection_distance;
        intersections_found++;
        if(intersections_found == 2)
        {
          break;
        }
      }

    }
  }

  if(intersections_found == 0) 
  {
    return std::nullopt;
  }
  else if(intersections_found == 1)
  {
    dist[1] = 0;
  }

  if(dist[0] > dist[1])
  {
    double dist0 = dist[0];
    dist[0] = dist[1];
    dist[1] = dist0;
  }

  Vector point0 = global_to_local_point( ray.origin() + ray.direction() * dist[0] );
  Vector point1 = global_to_local_point( ray.origin() + ray.direction() * dist[1]  );
  
  Plane intersection_plane;
  if(shape_type() == SURFACE) 
  {
    intersect_function(_f0, _f0_x, _f0_y, point0, point1, intersection_plane);
    return intersection_plane;
  }
  else if(shape_type() == SOLID)
  {
    if(intersections_found == 2 && intersect_container_boundary(point0, intersection_plane))
    {
      return intersection_plane;
    }
    else 
    {
      Plane plane0, plane1;
      bool is_intersection_0 = intersect_function(_f0, _f0_x, _f0_y, point0, point1, plane0);
      bool is_intersection_1 = intersect_function(_f1, _f1_x, _f1_y, point0, point1, plane1);
      double result0 = (plane0.origin() - ray.origin()).norm();
      double result1 = (plane1.origin() - ray.origin()).norm();

      if(is_intersection_0 && (!is_intersection_1 || result0 < result1))
      {
        return plane0;
      }
      else if(is_intersection_1)
      {
        return plane1;
      }
      else if(intersect_container_boundary(point1, intersection_plane))
      {
        return intersection_plane;
      }
    }
    return std::nullopt;
  }
  else
  {
    throw std::logic_error("FunctionPlot: Unknown shape_type.");
  }  
}

bool FunctionPlot::intersect_container_boundary(const Vector &point, Plane &intersection_plane) const
{
  double f0 = _f0(point);
  double f1 = _f1(point);
  if(is_smaller_or_equal(point.z(), f1) && is_greater_or_equal(point.z(), f0) &&
      is_smaller(f0, f1) &&
      is_greater(f1, _zmin) && is_smaller(f0, _zmax)) 
  {
    Vector u;
    if(is_equal(point.x(), _xmax)) {
      u = _plane[PLANE_XMAX].normal();
    }
    else if(is_equal(point.y(), _ymax)) {
      u = _plane[PLANE_YMAX].normal();
    }
    else if(is_equal(point.z(), _zmax)) {
      u = _plane[PLANE_ZMAX].normal();
    }
    else if(is_equal(point.z(), _zmin)) {
      u = _plane[PLANE_ZMIN].normal();
    }
    else if(is_equal(point.y(), _ymin)) {
      u = _plane[PLANE_YMIN].normal();
    }
    else if(is_equal(point.x(), _xmin)) {
      u = _plane[PLANE_XMIN].normal();
    }
    else {
      display_warning("FunctionPlot: Bounding Plane not found.");
    }
    intersection_plane = Plane(local_to_global_point(point), local_to_global_direction(u));
    return true;
  }
  else {
    return false;
  }
}

bool FunctionPlot::intersect_function(const MathExpression &f, const MathExpression &fx, const MathExpression &fy,
                                      Vector p0, const Vector &p1, Plane &intersection_plane) const
{
  static const double tol = 1e-6;

  while(is_greater( (p1-p0).norm(), 0, tol)) {

    auto func = [f=&f, p0=p0, p1=p1](double t)
    {
      Vector v = (1-t) * p0 + t * p1;
      return v.z() - (*f)(v.x(), v.y()); 
    };

    auto func_t = [fx=&fx, fy=&fy, p0=p0, p1=p1](double t)
    {
      Vector v = (1-t) * p0 + t * p1;
      Vector v_t = p1 - p0;
      return v_t.z() - (v_t.x() * (*fx)(v.x(), v.y()) + v_t.y() * (*fy)(v.x(), v.y()));
    };

    double distance;

    if(_root_finder.find(func, func_t, tol, 1-tol, distance)) {

      Vector p_local = (1-distance) * p0 + distance * p1;

      if(is_smaller(_f0(p_local), _f1(p_local))) {
        Vector p_global = local_to_global_point(p_local);
        Vector u_global = local_to_global_direction(Vector(1, 0, fx(p_local))) ^ local_to_global_direction(Vector(0, 1, fy(p_local)));
        intersection_plane = Plane(p_global, u_global);
        return true;
      }
      else {
        p0 = p_local;
      }
    }
    else {
      return false;
    }
  }

  return false;
}

bool FunctionPlot::inside(const Ray &ray) const
{
  if(shape_type() == SURFACE)
  {
    return false;
  }
  else if(shape_type() == SOLID)
  {
    Vector p = global_to_local_point(ray.origin() + 0.000001 * ray.direction());
    Vector u = global_to_local_direction(ray.direction());
    if(is_smaller(p.x(), _xmax) && is_greater(p.x(), _xmin) &&
       is_smaller(p.y(), _ymax) && is_greater(p.y(), _ymin) &&
       is_smaller(p.z(), _zmax) && is_greater(p.z(), _zmin) &&
       is_smaller_or_equal(p.z(), _f1(p)) && is_greater_or_equal(p.z(), _f0(p)) )
    {
      return true;
    }

    return false;
  }
  else
  {
    throw std::logic_error("FunctionPlot: Unexpected shape_type.");
  }
}

Color FunctionPlot::get_color(const Vector &vector) const
{
  return color();
}



