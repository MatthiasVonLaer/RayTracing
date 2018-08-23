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

#include "vector.h"

using namespace std;

const double Vector::operator[](int i) const
{
  if      (i==0) {
    return _x; 
  }
  else if (i==1) {
    return _y;
  }
  else if (i==2) {
    return _z;
  }
  else {
    throw std::out_of_range("Vector::operator[]: out of range");
  }
}

const Vector Vector::normalized() const
{	
  double d = norm();

  if(is_greater(d, 0))
    return Vector(_x/d, _y/d, _z/d);
  else
    return Vector();
}

void Vector::normalize() {
  double d = norm();

  if(is_greater(d, 0))
    set(_x/d, _y/d, _z/d);
  else
    set(0, 0, 0);
}

const string Vector::str() const {
  std::stringstream stream;
  stream << "Vector(" << _x << ", " << _y << ", " << _z << ")";
  return stream.str();
}


istream& operator>>(istream &in, Vector &v)
{
  double a, b, c;
  in >> a >> b >> c;
  v.set(a, b, c);
  return in;
}
