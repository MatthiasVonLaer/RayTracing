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
#include <sstream>
#include <math.h>

#include "utilities.h"

class Vector
{
private:
  double _x;
  double _y;
  double _z;

public:
  Vector()                                         :_x(0), _y(0), _z(0) {}
  Vector(double x, double y=0, double z=0)         :_x(x), _y(y), _z(z) {}
  
  void set(double x_in, double y_in, double z_in)  {_x=x_in; _y=y_in; _z=z_in;}
  void set_x(double d)                             {_x=d;}
  void set_y(double d)                             {_y=d;}
  void set_z(double d)                             {_z=d;}

  double x() const                                 {return _x;}
  double y() const                                 {return _y;}
  double z() const                                 {return _z;}

  double norm() const                              {return sqrt(_x*_x + _y*_y + _z*_z);}
  double norm2() const                             {return _x*_x + _y*_y + _z*_z;}
  Vector normalized() const;
  void normalize();
	
  //Operatoren
  void   operator+=(const Vector &cv)              {_x+=cv.x(); _y+=cv.y(); _z+=cv.z();}
  void   operator-=(const Vector &cv)              {_x-=cv.x(); _y-=cv.y(); _z-=cv.z();}

  Vector operator+(const Vector &cv) const         {return Vector(_x+cv.x(), _y+cv.y(), _z+cv.z());}
  Vector operator-(const Vector &cv) const         {return Vector(_x-cv.x(), _y-cv.y(), _z-cv.z());}
  double operator*(const Vector &cv) const         {return _x*cv.x() + _y*cv.y() + _z*cv.z();}
  Vector operator^(const Vector &cv) const         {return Vector(_y*cv.z()-_z*cv.y(), _z*cv.x()-_x*cv.z(), _x*cv.y()-_y*cv.x());}
  bool   operator==(const Vector &cv) const        {return is_equal(_x, cv.x()) && is_equal(_y, cv.y()) && is_equal(_z, cv.z());}
  bool	 operator!=(const Vector &cv) const        {return !is_equal(_x, cv.x()) || !is_equal(_y, cv.y()) || !is_equal(_z, cv.z());}

  void   operator*=(double d)                      {_x*=d; _y*=d; _z*=d;}
  Vector operator*(double d) const                 {return Vector(_x*d, _y*d, _z*d);}
  Vector operator/(double d) const                 {return Vector(_x/d, _y/d, _z/d);}
  Vector operator-(void) const                     {return Vector(-_x, -_y, -_z);}
  double operator[](int i) const                   {if(i==0) return _x; else if(i==1) return _y; else if (i==2) return _z;}

  std::string str() const;
};

Vector operator*(double d, const Vector &v);
std::istream& operator>>(std::istream &in, Vector &v);

