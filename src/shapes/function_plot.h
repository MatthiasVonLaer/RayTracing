#pragma once
#include <vector>
#include <complex>

#include "math_expression.h"
#include "root_finder.h"
#include "shape.h"

class FunctionPlot : public Shape
{
public:
  FunctionPlot(const Composition *parent);
  virtual void parse(const std::string &command, std::istream &in);

protected:
  virtual void init_derived();
	
public:
  virtual bool intersect(const Ray &ray, Plane &plane) const;
  virtual bool inside(const Ray &ray) const;
  virtual Color get_color(const Vector &vector) const;

private:
  bool intersect_container_boundary(const Vector &point, Plane &intersection_plane) const;
  bool intersect_function(const MathExpression &f, const MathExpression &fx, const MathExpression &fy,
                          const Vector &p0, const Vector &p1, Plane &intersection_plane) const;

private:
  Plane _plane[6];
  QImage _image;
  MathExpression _f0, _f0_x, _f0_y;
  MathExpression _f1, _f1_x, _f1_y;
  RootFinder _root_finder;

  double _xmin, _xmax;
  double _ymin, _ymax;
  double _zmin, _zmax;

  enum {PLANE_XMAX=0, PLANE_YMAX=1, PLANE_ZMAX=2, PLANE_ZMIN=3, PLANE_YMIN=4, PLANE_XMIN=5};

private:
  static double wrap_function(double t);
  static double wrap_function_t(double t);

  static Vector                wrap_intersection_0;
  static Vector                wrap_intersection_1;
  static const MathExpression *wrap_f;
  static const MathExpression *wrap_f_x;
  static const MathExpression *wrap_f_y;
};
