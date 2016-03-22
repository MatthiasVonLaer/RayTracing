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

#include <fstream>

#include "vector.h"

class Plane
{
private:
  Vector _p;
  Vector _n;

public:
  Plane()                                           {_p = Vector(); _n = Vector();}
  Plane(const Vector &p, const Vector &n)           {_p = p; _n = n.normalized();}

  Vector origin() const                             {return _p;}
  Vector normal() const                             {return _n;}
};
