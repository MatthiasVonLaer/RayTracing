#include "vector.h"

using namespace std;

Vector Vector::normalized() const
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

string Vector::str() const {
  std::stringstream stream;
  stream << "Vector(" << _x << ", " << _y << ", " << _z << ")";
  return stream.str();
}

Vector operator*(double d, const Vector &v)
{
  return Vector(v.x()*d, v.y()*d, v.z()*d);
}

istream& operator>>(istream &in, Vector &v)
{
  double a, b, c;
  in >> a >> b >> c;
  v.set(a, b, c);
  return in;
}
