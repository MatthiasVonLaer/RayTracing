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
