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

#pragma once

#include "math_expression.h"
#include "root_finder.h"
#include "shape.h"

#include <complex>
#include <vector>

class FunctionPlot : public Shape
{
private:
  enum {PLANE_XMAX=0, PLANE_YMAX=1, PLANE_ZMAX=2, PLANE_ZMIN=3, PLANE_YMIN=4, PLANE_XMIN=5};

private:
  RootFinder _root_finder;

  Plane _plane[6];
  QImage _image;
  MathExpression _f0, _f0_x, _f0_y;
  MathExpression _f1, _f1_x, _f1_y;

  double _xmin, _xmax;
  double _ymin, _ymax;
  double _zmin, _zmax;


public:
  FunctionPlot(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in) override;

private:
  double init_dx() override {return width()  / (_xmax - _xmin);}
  double init_dy() override {return depth()  / (_ymax - _ymin);}
  double init_dz() override {return height() / (_zmax - _zmin);}
  Vector init_origin() override;
  void init_derived_class() override;
	
public:
  std::optional<Plane> intersect(const Ray &ray) const override;
private:
  bool intersect_container_boundary(const Vector &point, Plane &intersection_plane) const;
  bool intersect_function(const MathExpression &f, const MathExpression &fx, const MathExpression &fy,
                          Vector p0, const Vector &p1, Plane &intersection_plane) const;
public:
  bool inside(const Ray &ray) const override;
  Color get_color(const Vector &vector) const override;
};
