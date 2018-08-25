//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "vector.h"

#include <string>

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

  const double det() const;
  const Matrix inv() const;

  const Matrix operator*(const Matrix &m) const;
  const Vector operator*(const Vector &v) const;
  const Matrix operator*(double d) const;
  const Matrix operator/(double d) const;
  const double operator()(int i, int j) const;

  const std::string str() const;

private:
  const double cofactor(int i, int j) const;
};
