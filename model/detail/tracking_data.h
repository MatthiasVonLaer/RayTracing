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

#include "ray.h"
#include "shapes/shape.h"

class TrackingData
{
public:
  Ray ray;
  const Shape *shape;
  double distance;
  double ratio;

public:
  TrackingData(const Ray &ray_in, const Shape *shape_in, double distance_in, double ratio_in) :
    ray(ray_in), shape(shape_in), distance(distance_in), ratio(ratio_in) {}
};
