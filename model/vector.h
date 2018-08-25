//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "utilities.h"

class Vector
{
private:
  double _x;
  double _y;
  double _z;

public:
  explicit Vector(double x=0, double y=0) :_x(x), _y(y), _z(0) {}
  Vector(double x, double y, double z) :_x(x), _y(y), _z(z) {}
  
  void set(double x_in, double y_in, double z_in)  {_x=x_in; _y=y_in; _z=z_in;}
  void set_x(double d)                             {_x=d;}
  void set_y(double d)                             {_y=d;}
  void set_z(double d)                             {_z=d;}

  const double x() const                           {return _x;}
  const double y() const                           {return _y;}
  const double z() const                           {return _z;}
  const double operator[](int i) const;

  const double norm() const                        {return sqrt(_x*_x + _y*_y + _z*_z);}
  const double norm2() const                       {return _x*_x + _y*_y + _z*_z;}
  const Vector normalized() const;
  void normalize();
	
  Vector& operator+=(const Vector &cv)              {_x+=cv.x(); _y+=cv.y(); _z+=cv.z(); return *this;}
  Vector& operator-=(const Vector &cv)              {_x-=cv.x(); _y-=cv.y(); _z-=cv.z(); return *this;}
  Vector& operator*=(double d)                      {_x*=d; _y*=d; _z*=d; return *this;}
  Vector& operator/=(double d)                      {_x/=d; _y/=d; _z/=d; return *this;}

  const std::string str() const;
};


inline const double operator* (const Vector &lhs, const Vector &rhs)
{
  return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
}

inline const Vector operator+ (const Vector &lhs, const Vector &rhs)
{
  return Vector(lhs.x()+rhs.x(),  lhs.y()+rhs.y(),  lhs.z()+rhs.z());
}

inline const Vector operator- (const Vector &lhs, const Vector &rhs)
{
  return Vector(lhs.x()-rhs.x(),  lhs.y()-rhs.y(),  lhs.z()-rhs.z());
}

inline const Vector operator^ (const Vector &lhs, const Vector &rhs) {
  return Vector(lhs.y()*rhs.z()-lhs.z()*rhs.y(),
      lhs.z()*rhs.x()-lhs.x()*rhs.z(),
      lhs.x()*rhs.y()-lhs.y()*rhs.x());
}

inline const bool   operator==(const Vector &lhs, const Vector &rhs) {
  return is_equal(lhs.x(), rhs.x()) &&
         is_equal(lhs.y(), rhs.y()) &&
         is_equal(lhs.z(), rhs.z());
}

inline const bool   operator!=(const Vector &lhs, const Vector &rhs) {
  return !is_equal(lhs.x(), rhs.x()) ||
         !is_equal(lhs.y(), rhs.y()) ||
         !is_equal(lhs.z(), rhs.z());
}

inline const Vector operator*(const Vector &lhs, double d)          
{
  return Vector(lhs.x()*d,  lhs.y()*d,  lhs.z()*d);
}

inline const Vector operator*(double d, const Vector &rhs)          
{
  return Vector(rhs.x()*d,  rhs.y()*d,  rhs.z()*d);
}

inline const Vector operator/(const Vector &lhs, double d)          
{
  return Vector(lhs.x()/d,  lhs.y()/d,  lhs.z()/d);
}

inline const Vector operator-(const Vector &v)                      
{
  return Vector(-v.x(),  -v.y(),  -v.z());
}


std::istream& operator>>(std::istream &in, Vector &v);

