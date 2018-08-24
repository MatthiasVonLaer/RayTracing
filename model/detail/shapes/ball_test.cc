#include "ball.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Raytracing_Ball_intersect_yesOrNo )
{
  Ball ball;
  BOOST_TEST(ball.intersect(Ray({0, 0, 0}, {0, 0, 1})).has_value());
  BOOST_TEST(ball.intersect(Ray({0, 0, -2}, {0, 0, 1})).has_value());
  BOOST_TEST(!ball.intersect(Ray({0, 0, 2}, {0, 0, 1})).has_value());
}

BOOST_AUTO_TEST_CASE( Raytracing_Ball_intersect_intersectionPlane )
{
  Ball ball;
  const auto plane = ball.intersect(Ray({0, 0, -2}, {0, 0, 1}));
  BOOST_TEST(plane.has_value());
  BOOST_CHECK_EQUAL(Plane({0, 0, -1}, {0, 0, -1}), *plane);
}
