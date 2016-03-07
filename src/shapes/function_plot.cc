#include <iostream>
#include <vector>

#include "function_plot.h"
#include "utilities.h"

using namespace std;

FunctionPlot::FunctionPlot() :
  _xmin(-1),
  _ymin(-1),
  _zmin(-1),
  _xmax(1),
  _ymax(1),
  _zmax(1)
{	
  _shape_type = SURFACE;
}

void FunctionPlot::init()
{
  if(is_greater_or_equal(_xmin, _xmax) ||is_greater_or_equal(_ymin, _ymax) ||  is_greater_or_equal(_zmin, _zmax)) {
    display_error("FunctionPlot: min >= max");
  }

  _dx *= 2 / (_xmax - _xmin);
  _dy *= 2 / (_ymax - _ymin);
  _dz *= 2 / (_zmax - _zmin);

  _position -= (_dx*(_xmax+_xmin)/2) * vx();
  _position -= (_dy*(_ymax+_ymin)/2) * vy();
  _position -= (_dz*(_zmax+_zmin)/2) * vz();

  _plane[PLANE_XMAX] = Plane( _position + _dx*_xmax * vx(), vx());
  _plane[PLANE_YMAX] = Plane( _position + _dy*_ymax * vy(), vy());
  _plane[PLANE_ZMAX] = Plane( _position + _dz*_zmax * vz(), vz());
  _plane[PLANE_ZMIN] = Plane( _position + _dz*_zmin * vz(), -vz());
  _plane[PLANE_YMIN] = Plane( _position + _dy*_ymin * vy(), -vy());
  _plane[PLANE_XMIN] = Plane( _position + _dx*_xmin * vx(), -vx());
}

void FunctionPlot::parse(const string &command, istream &in)
{
  if(command == "function_0") {
    string expr;
    getline(in, expr);
    _f0.set(expr);
    _f0_x = _f0.dx();
    _f0_y = _f0.dy();
  }
  else if(command == "function_1") {
    _shape_type = SOLID;
    string expr;
    getline(in, expr);
    _f1.set(expr);
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

bool FunctionPlot::intersect(const Ray &ray, Plane &intersection_plane) const
{
  double dist[2];
  int intersections_found = 0;

  for(int i=0; i<6; i++)
  {
    double intersection_distance;
    if(ray.intersect(_plane[i], intersection_distance)) {
      Vector point = global_to_local_point(ray.origin() + intersection_distance * ray.direction());
	
      if(is_greater_or_equal(_xmax, point.x()) && is_greater_or_equal(_ymax, point.y()) && is_greater_or_equal(_zmax, point.z()) &&
         is_smaller_or_equal(_xmin, point.x()) && is_smaller_or_equal(_ymin, point.y()) && is_smaller_or_equal(_zmin, point.z()) )
      {
        dist[intersections_found] = intersection_distance;
        intersections_found++;
        if(intersections_found == 2) {
          break;
        }
      }

    }
  }

  if(intersections_found == 0) {
    return false;
  }
  else if(intersections_found == 1) {
    dist[1] = 0;
  }

  if(dist[0] > dist[1]) {
    double dist0 = dist[0];
    dist[0] = dist[1];
    dist[1] = dist0;
  }

  Vector point0 = global_to_local_point( ray.origin() + ray.direction() * dist[0] );
  Vector point1 = global_to_local_point( ray.origin() + ray.direction() * dist[1]  );
  
  if(_shape_type == SURFACE) {
    intersect_function(_f0, _f0_x, _f0_y, point0, point1, intersection_plane);
  }
  else if(_shape_type == SOLID) {
    if(intersections_found == 2 && intersect_container_boundary(point0, intersection_plane)) {
      return true;
    }
    else {
      Plane plane0, plane1;
      bool is_intersection_0 = intersect_function(_f0, _f0_x, _f0_y, point0, point1, plane0);
      bool is_intersection_1 = intersect_function(_f1, _f1_x, _f1_y, point0, point1, plane1);
      double result0 = (plane0.origin() - ray.origin()).norm();
      double result1 = (plane1.origin() - ray.origin()).norm();

      if(is_intersection_0 && (!is_intersection_1 || result0 < result1)) {
        intersection_plane = plane0;
        return true;
      }
      else if(is_intersection_1) {
        intersection_plane = plane1;
        return true;
      }
      else if(intersect_container_boundary(point1, intersection_plane)) {
        return true;
      }
    }
    return false;
  }
  else {
    display_error("FunctionPlot: Unknown shape_type.");
  }  
}

bool FunctionPlot::inside(const Ray &ray) const
{
  if(_shape_type == SURFACE) {
    return false;
  }
  else if(_shape_type == SOLID) {
    Vector p = global_to_local_point(ray.origin());
    Vector u = global_to_local_direction(ray.direction());
    if(is_smaller(p.x(), _xmax) && is_greater(p.x(), _xmin) &&
       is_smaller(p.y(), _ymax) && is_greater(p.y(), _ymin) &&
       is_smaller(p.z(), _zmax) && is_greater(p.z(), _zmin))
    {
      if(is_greater(_f0(p), _f1(p))) {
        if(is_smaller(p.z(), _f0(p)) && is_greater(p.z(), _f1(p))) {
          return true;
        }
        else if(is_equal(p.z(), _f0(p)) && is_greater(0, u * (Vector(1, 0, _f0_x(p)) ^ Vector(0, 1, _f0_y(p))))) {
          return true;
        }
        else if(is_equal(p.z(), _f1(p)) && is_smaller(0, u * (Vector(1, 0, _f1_x(p)) ^ Vector(0, 1, _f1_y(p))))) {
          return true;
        }
      }
      else if(is_greater(_f1(p), _f0(p))) {
        if(is_smaller(p.z(), _f1(p)) && is_greater(p.z(), _f0(p))) {
          return true;
        }
        else if(is_equal(p.z(), _f1(p)) && is_greater(0, u * (Vector(1, 0, _f1_x(p)) ^ Vector(0, 1, _f1_y(p))))) {
          return true;
        }
        else if(is_equal(p.z(), _f0(p)) && is_smaller(0, u * (Vector(1, 0, _f0_x(p)) ^ Vector(0, 1, _f0_y(p))))) {
          return true;
        }
      }
    }
    else if(is_smaller_or_equal(p.x(), _xmax) && is_greater_or_equal(p.x(), _xmin) &&
            is_smaller_or_equal(p.y(), _ymax) && is_greater_or_equal(p.y(), _ymin) &&
            is_smaller_or_equal(p.z(), _zmax) && is_greater_or_equal(p.z(), _zmin))
    {
      if((is_smaller_or_equal(p.z(), _f0(p)) && is_greater_or_equal(p.z(), _f1(p)) ) ||
         (is_smaller_or_equal(p.z(), _f1(p)) && is_greater_or_equal(p.z(), _f0(p)) )) 
      {
        if     (is_equal(p.x(), _xmax) && is_smaller(u.x(), 0)) {
          return true;
        }
        else if(is_equal(p.x(), _xmin) && is_greater(u.x(), 0)) {
          return true;
        }
        else if(is_equal(p.y(), _ymax) && is_smaller(u.y(), 0)) {
          return true;
        }
        else if(is_equal(p.y(), _ymin) && is_greater(u.y(), 0)) {
          return true;
        }
        else if(is_equal(p.z(), _zmax) && is_smaller(u.z(), 0)) {
          return true;
        }
        else if(is_equal(p.z(), _zmin) && is_greater(u.z(), 0)) {
          return true;
        }
      }
    }

    return false;
  }
  else {
    display_error("FunctionPlot: Unexpected shape_type.");
  }
}

Color FunctionPlot::get_color(const Vector &vector) const
{
  return _color;
  /*
  double x = (vector-center) * lineTo0.Normalized();
  double y = (vector-center) * lineTo1.Normalized();
  double z = (vector-center) * lineTo2.Normalized();
  
  if(colorTyp == C_FARBIG && !valueColor && !isComplex) {
    if(CompareD(f(f1, x, y), z) == GL)
      return color1;
    else if(solidBody && CompareD(f(f2, x, y), z) == GL) {
      return color2;
    }
    else
      return color0;
  }

  else if(colorTyp == C_FARBIG && isComplex) {
    double r, g, b;
    double phi = 3 * (.5 + arg(h(x, y))/(2*PI) );
    if(phi <= 1) {
      r = cos(PI/2*phi);
      g = sin(PI/2*phi);
      b = 0;
    }
    else if(phi <= 2) {
      phi--;
      r = 0;
      g = cos(PI/2*phi);
      b = sin(PI/2*phi);
    }
    else {
      phi -= 2;
      r = sin(PI/2*phi);
      g = 0;
      b = cos(PI/2*phi);
    }
    return QColor(r*255, g*255, b*255);
  }
  else if(colorTyp == C_FARBIG && valueColor) {
    if(CompareD(f(f1, x, y), z) == GL) {
      double d;
      if(z < vcMin)
	d = 0;
      else if(z > vcMax)
	d = 1;
      else
	d = (z - vcMin) / (vcMax - vcMin);
      return QColor( ((1-d)*color1.red()+d*color2.red())/2,  ((1-d)*color1.green()+d*color2.green())/2,  ((1-d)*color1.blue()+d*color2.blue())/2 );
    }
    else
      return color0;
  }
	
  else if(colorTyp == C_BITMAP) {
    double a = (x/length0 + 1)/2;
    double b = (y/length1 + 1)/2;
    if(CompareD(f(f1, x, y), z) == GL)
      return image.pixel( a * image.width(), b * image.height() );
    else if(solidBody && CompareD(f(f2, x, y), z) == GL)
      return color2;
    else
      return color0;
    return QColor(255, 255, 255);
  }
  */
}

bool FunctionPlot::intersect_container_boundary(const Vector &point, Plane &intersection_plane) const
{
  if((is_smaller_or_equal(point.z(), _f0(point)) && is_greater_or_equal(point.z(), _f1(point)) ) ||
     (is_smaller_or_equal(point.z(), _f1(point)) && is_greater_or_equal(point.z(), _f0(point)) )) 
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
                                      const Vector &p0, const Vector &p1, Plane &intersection_plane) const
{
  wrap_f   = &f;
  wrap_f_x = &fx;
  wrap_f_y = &fy;
  wrap_intersection_0 = p0;
  wrap_intersection_1 = p1;
  double L = 100;
  double distance;

  if(_root_finder.find(&wrap_function, &wrap_function_t, L, 1e-6, 1, distance)) {

    Vector p_local = (1-distance) * wrap_intersection_0 + distance * wrap_intersection_1;
    Vector p_global = local_to_global_point(p_local);
    Vector u_global = local_to_global_direction(Vector(1, 0, fx(p_local))) ^ local_to_global_direction(Vector(0, 1, fy(p_local)));
    intersection_plane = Plane(p_global, u_global);
    return true;

  }
  else {
    return false;
  }
}


double FunctionPlot::wrap_function(double t)
{
  Vector v = (1-t) * wrap_intersection_0 + t * wrap_intersection_1;
  return v.z() - (*wrap_f)(v.x(), v.y()); 
}

double FunctionPlot::wrap_function_t(double t)
{
  Vector v = (1-t) * wrap_intersection_0 + t * wrap_intersection_1;
  Vector v_t = wrap_intersection_1 - wrap_intersection_0;
  return v_t.z() - (v_t.x() * (*wrap_f_x)(v.x(), v.y()) + v_t.y() * (*wrap_f_y)(v.x(), v.y()));
}

Vector                FunctionPlot::wrap_intersection_0;
Vector                FunctionPlot::wrap_intersection_1;
const MathExpression *FunctionPlot::wrap_f;
const MathExpression *FunctionPlot::wrap_f_x;
const MathExpression *FunctionPlot::wrap_f_y;
