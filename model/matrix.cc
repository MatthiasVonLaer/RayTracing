//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "matrix.h"

#include <sstream>

using namespace std;

Matrix::Matrix(double a00, double a01, double a02,
               double a10, double a11, double a12,
               double a20, double a21, double a22)
{
  _a[0][0] = a00;
  _a[0][1] = a01;
  _a[0][2] = a02;
  _a[1][0] = a10;
  _a[1][1] = a11;
  _a[1][2] = a12;
  _a[2][0] = a20;
  _a[2][1] = a21;
  _a[2][2] = a22;
}

Matrix::Matrix(double A[3][3])
{
  _a[0][0] = A[0][0];
  _a[0][1] = A[0][1];
  _a[0][2] = A[0][2];
  _a[1][0] = A[1][0];
  _a[1][1] = A[1][1];
  _a[1][2] = A[1][2];
  _a[2][0] = A[2][0];
  _a[2][1] = A[2][1];
  _a[2][2] = A[2][2];
}

Matrix::Matrix(const Vector &v0, const Vector &v1, const Vector &v2)
{
  _a[0][0] = v0[0];
  _a[0][1] = v1[0];
  _a[0][2] = v2[0];
  _a[1][0] = v0[1];
  _a[1][1] = v1[1];
  _a[1][2] = v2[1];
  _a[2][0] = v0[2];
  _a[2][1] = v1[2];
  _a[2][2] = v2[2];
}

Matrix::Matrix(double d)
{
  _a[0][0] = d;
  _a[0][1] = 0;
  _a[0][2] = 0;
  _a[1][0] = 0;
  _a[1][1] = d;
  _a[1][2] = 0;
  _a[2][0] = 0;
  _a[2][1] = 0;
  _a[2][2] = d;
}

Matrix::Matrix()
{
  _a[0][0] = 0;
  _a[0][1] = 0;
  _a[0][2] = 0;
  _a[1][0] = 0;
  _a[1][1] = 0;
  _a[1][2] = 0;
  _a[2][0] = 0;
  _a[2][1] = 0;
  _a[2][2] = 0;
}


const double Matrix::det() const
{
  return   _a[0][0] * (_a[1][1]*_a[2][2] - _a[1][2]*_a[2][1])
         - _a[0][1] * (_a[1][0]*_a[2][2] - _a[1][2]*_a[2][0])
         + _a[0][2] * (_a[1][0]*_a[2][1] - _a[1][1]*_a[2][0]);
}

// ( a[0][0]  a[0][1]  a[0][2] )
// ( a[1][0]  a[1][1]  a[1][2] )
// ( a[2][0]  a[2][1]  a[2][2] )

const Matrix Matrix::inv() const
{
  return Matrix(_a[1][1]*_a[2][2] - _a[2][1]*_a[1][2],
                _a[2][1]*_a[0][2] - _a[0][1]*_a[2][2],
                _a[0][1]*_a[1][2] - _a[1][1]*_a[0][2],
                _a[2][0]*_a[1][2] - _a[1][0]*_a[2][2],
                _a[0][0]*_a[2][2] - _a[0][2]*_a[2][0],
                _a[1][0]*_a[0][2] - _a[0][0]*_a[1][2],
                _a[1][0]*_a[2][1] - _a[1][1]*_a[2][0],
                _a[2][0]*_a[0][1] - _a[0][0]*_a[2][1],
                _a[0][0]*_a[1][1] - _a[1][0]*_a[0][1])/det();
}

const Matrix Matrix::operator*(const Matrix &m) const
{
  return Matrix(_a[0][0]*m(0,0) + _a[0][1]*m(1,0) + _a[0][2]*m(2,0),
                _a[0][0]*m(0,1) + _a[0][1]*m(1,1) + _a[0][2]*m(2,1),
                _a[0][0]*m(0,2) + _a[0][1]*m(1,2) + _a[0][2]*m(2,2),
                _a[1][0]*m(0,0) + _a[1][1]*m(1,0) + _a[1][2]*m(2,0),
                _a[1][0]*m(0,1) + _a[1][1]*m(1,1) + _a[1][2]*m(2,1),
                _a[1][0]*m(0,2) + _a[1][1]*m(1,2) + _a[1][2]*m(2,2),
                _a[2][0]*m(0,0) + _a[2][1]*m(1,0) + _a[2][2]*m(2,0),
                _a[2][0]*m(0,1) + _a[2][1]*m(1,1) + _a[2][2]*m(2,1),
                _a[2][0]*m(0,2) + _a[2][1]*m(1,2) + _a[2][2]*m(2,2));
}

const Vector Matrix::operator*(const Vector &v) const
{
  return Vector(_a[0][0]*v.x() + _a[0][1]*v.y() + _a[0][2]*v.z(),
                _a[1][0]*v.x() + _a[1][1]*v.y() + _a[1][2]*v.z(),
                _a[2][0]*v.x() + _a[2][1]*v.y() + _a[2][2]*v.z());
}

const Matrix Matrix::operator*(double d) const
{
  return Matrix(_a[0][0]*d, _a[0][1]*d, _a[0][2]*d, 
                _a[1][0]*d, _a[1][1]*d, _a[1][2]*d,
                _a[2][0]*d, _a[2][1]*d, _a[2][2]*d);
}

const Matrix Matrix::operator/(double d) const
{
  if(is_equal(d, 0)) {
    display_warning("Matrix divided by zero.");
    return Matrix();
  }
  else {
    return Matrix(_a[0][0]/d, _a[0][1]/d, _a[0][2]/d,
                  _a[1][0]/d, _a[1][1]/d, _a[1][2]/d,
                  _a[2][0]/d, _a[2][1]/d, _a[2][2]/d);
  }
}

const double Matrix::operator()(int i, int j) const
{
  return _a[i][j];
}

const string Matrix::str() const
{
  stringstream stream;
  stream << endl;
  stream << _a[0][0] << " " << _a[0][1] << " " << _a[0][2] << endl;
  stream << _a[1][0] << " " << _a[1][1] << " " << _a[1][2] << endl;
  stream << _a[2][0] << " " << _a[2][1] << " " << _a[2][2] << endl;
  return stream.str();
}

const double Matrix::cofactor(int i, int j) const
{
  return _a[(i+1)%3][(j+1)%3] * _a[(i+2)%3][(j+2)%3] - _a[(i+2)%3][(j+1)%3] * _a[(i+1)%3][(j+2)%3];
}
