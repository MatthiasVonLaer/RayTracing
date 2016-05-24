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
#include "vector.h"

class Ray
{
private:
  Vector p;
  Vector u;

public:
  Ray(){}
  Ray(const Vector& v1, const Vector& v2) {p = v1; u = v2.normalized();}

  const Ray operator+(const Vector &vector) const {return Ray(p+vector, u);}

  bool intersect(const Plane &plane, double &intersection_distance) const;

  Vector origin()const                    {return p;}
  Vector direction()const                 {return u;}
  
  void set_origin(const Vector &v)        {p = v;}
  void set_direction(const Vector &v)     {u = v.normalized();}
};
