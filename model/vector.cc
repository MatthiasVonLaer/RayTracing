//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
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
