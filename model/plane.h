//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "vector.h"

class Plane
{
public:
  explicit Plane() = default;
  Plane(const Vector& origin, const Vector& normal);

  Vector origin() const {return _p;}
  Vector normal() const {return _n;}

private:
  Vector _p;
  Vector _n;
};

double distance(const Plane& lhs, const Vector& rhs);
bool operator==(const Plane& lhs, const Plane& rhs);
std::ostream& operator<<(std::ostream& out, const Plane& plane);
