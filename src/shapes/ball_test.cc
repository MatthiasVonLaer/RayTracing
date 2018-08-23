#include "ball.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Raytracing_Ball_intersect )
{
  Ball ball;
  Plane plane;
  BOOST_TEST(ball.intersect(Ray({0, 0, 0}, {0, 0, 1})).has_value());
  BOOST_TEST(ball.intersect(Ray({0, 0, -2}, {0, 0, 1})).has_value());
  BOOST_TEST(!ball.intersect(Ray({0, 0, 2}, {0, 0, 1})).has_value());
}
