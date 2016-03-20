#pragma once
#include "plane.h"
#include "vector.h"

class Ray
{
private:
  Vector p;
  Vector u;

public:
  Ray(){}
  Ray(const Vector& v1, const Vector& v2) {p = v1; u = v2.normalized();}

  Ray operator+(const Vector &vector) const {return Ray(p+vector, u);}

  bool intersect(const Plane &plane, double &intersection_distance) const;

  Vector origin()const                    {return p;}
  Vector direction()const                 {return u;}
  
  void set_origin(const Vector &v)        {p = v;}
  void set_direction(const Vector &v)     {u = v.normalized();}
};
