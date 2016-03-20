#pragma once

#include <fstream>

#include "vector.h"

class Plane
{
private:
  Vector _p;
  Vector _n;

public:
  Plane()                                           {_p = Vector(); _n = Vector();}
  Plane(const Vector &p, const Vector &n)           {_p = p; _n = n.normalized();}

  Vector origin() const                             {return _p;}
  Vector normal() const                             {return _n;}
};
