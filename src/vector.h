#pragma once

#include <fstream>
#include <sstream>
#include <math.h>

#include "utilities.h"

class Vector
{
public:
  Vector()                                         :_x(0), _y(0), _z(0) {}
  Vector(double x_in, double y_in, double z_in)    :_x(x_in), _y(y_in), _z(z_in) {}
  
  void set(double x_in, double y_in, double z_in)  {_x=x_in; _y=y_in; _z=z_in;}
  //void set_x(double a)                             {_x=a;}
  //void set_y(double a)                             {_y=a;}
  //void set_z(double a)                             {_z=a;}

  double x() const                                 {return _x;}
  double y() const                                 {return _y;}
  double z() const                                 {return _z;}
  double norm() const                              {return sqrt(_x*_x + _y*_y + _z*_z);}
  double norm2() const                             {return _x*_x + _y*_y + _z*_z;}
  Vector normalized() const;
  void normalize();
  std::string str() const;
	
  //Operatoren
  void   operator+=(const Vector &cv)              {_x+=cv.x(); _y+=cv.y(); _z+=cv.z();}
  void   operator-=(const Vector &cv)              {_x-=cv.x(); _y-=cv.y(); _z-=cv.z();}

  Vector operator+(const Vector &cv) const         {return Vector(_x+cv.x(), _y+cv.y(), _z+cv.z());}
  Vector operator-(const Vector &cv) const         {return Vector(_x-cv.x(), _y-cv.y(), _z-cv.z());}
  double operator*(const Vector &cv) const         {return _x*cv.x() + _y*cv.y() + _z*cv.z();}
  Vector operator^(const Vector &cv) const         {return Vector(_y*cv.z()-_z*cv.y(), _z*cv.x()-_x*cv.z(), _x*cv.y()-_y*cv.x());}
  bool   operator==(const Vector &cv) const        {return is_equal(_x, cv.x()) && is_equal(_y, cv.y()) && is_equal(_z, cv.z());}
  bool	 operator!=(const Vector &cv) const        {return !is_equal(_x, cv.x()) || !is_equal(_y, cv.y()) || !is_equal(_z, cv.z());}

  Vector operator*(double d) const                 {return Vector(_x*d, _y*d, _z*d);}
  Vector operator/(double d) const                 {return Vector(_x/d, _y/d, _z/d);}
  Vector operator-(void) const                     {return Vector(-_x, -_y, -_z);}
  double operator[](int i) const                   {if(i==0) return _x; else if(i==1) return _y; else if (i==2) return _z;}

private:
  double _x;
  double _y;
  double _z;
};

Vector operator*(double d, const Vector &v);
std::istream& operator>>(std::istream &in, Vector &v);

