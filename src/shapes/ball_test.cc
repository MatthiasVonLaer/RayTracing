#include "ball.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Raytracing_Ball )
{
  Ball ball(nullptr);
  Plane plane;
  BOOST_TEST(ball.intersect(Ray(Vector(0, 0, -2), Vector(0, 0, 1)), plane));
  BOOST_TEST(!ball.intersect(Ray(Vector(0, 0, 2), Vector(0, 0, 1)), plane));
}
