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

#include "vector.h"

class Matrix
{
private:
  double _a[3][3];

public:
  Matrix(double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22);
  Matrix(double A[3][3]);
  Matrix(const Vector &v0, const Vector &v1, const Vector &v2);
  Matrix(double d);
  Matrix();

  double det() const;
  Matrix inv() const;

  Matrix operator*(const Matrix &m) const;
  Vector operator*(const Vector &v) const;
  Matrix operator*(double d) const;
  Matrix operator/(double d) const;
  double operator()(int i, int j) const;

  std::string str() const;

private:
  double cofactor(int i, int j) const;
};
