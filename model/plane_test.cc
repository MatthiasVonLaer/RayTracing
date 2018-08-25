#include "plane.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Raytracing_Plane_distance )
{
  const Plane plane({0, 0, 1}, {0, 0, 1});
  BOOST_CHECK_EQUAL(1, distance(plane, Vector()));
}
