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

#include <string>
#include <vector>

class Polygon
{
private:
  const int _number_vertices;
  const int _radius;
  const int _diameter;

  std::vector < std::vector < bool > > _pixelmatrix;
  std::vector < int > _pixelvector_x;
  std::vector < int > _pixelvector_y;

public:
  Polygon(int number_vertices, int radius);

private:
  void fill_matrix();
  void fill_vector();

public:
  int x(int n) const {return _pixelvector_x[n];}
  int y(int n) const {return _pixelvector_y[n];}
  int size() const   {return _pixelvector_x.size();}

  std::string str() const;
};
