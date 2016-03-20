#pragma once

#include "ray.h"
#include "shape.h"

class TrackingData
{
public:
  Ray ray;
  const Shape *shape;
  double distance;
  double ratio;

public:
  TrackingData(const Ray &ray_in, const Shape *shape_in, double distance_in, double ratio_in) :
    ray(ray_in), shape(shape_in), distance(distance_in), ratio(ratio_in) {}
};
