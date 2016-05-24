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

#include "plane.h"
#include "shape.h"
#include "vector.h"

#include <vector>

class Cylinder : public Shape
{
private:
  Plane _plane[2];
  double _radius;

public:
  Cylinder(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in) override;

private:
  double init_dx() override {return _radius;}
  double init_dy() override {return _radius;}
  void init_derived_class() override;
	
public:
  bool intersect(const Ray &ray, Plane &intersection_plane) const override;
private:
  bool intersect_plane(const Ray &ray, Plane &intersection_plane) const;
  bool intersect_circle(const Ray &ray, Plane &intersection_plane) const;
public:
  bool inside(const Ray &ray) const override;
  Color get_color(const Vector &intersection_point) const override;
};
