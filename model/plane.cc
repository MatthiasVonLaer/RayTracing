//  A raytracer program for rendering photorealistic images
//  Copyright 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "plane.h"

Plane::Plane(const Vector& origin, const Vector& normal)
  : _p(origin)
  , _n(normal.normalized())
{
}

double distance(const Plane& lhs, const Vector& rhs)
{
  return fabs((lhs.origin() - rhs) * lhs.normal());
}

bool operator==(const Plane& lhs, const Plane& rhs)
{
  constexpr auto tolerance = 1e-15;
  return (distance(lhs, rhs.origin()) < tolerance)
    && fabs(lhs.normal() * rhs.normal() - 1) < tolerance;
}

std::ostream& operator<<(std::ostream& out, const Plane& plane)
{
  const auto p = plane.origin();
  const auto n = plane.normal();
  out << "Plane [p = (" << p.x() << " " << p.y() << " " << p.z() << "), ";
  out << "n = (" << n.x() << " " << n.y() << " " << n.z() << ")]";
  return out;
}
