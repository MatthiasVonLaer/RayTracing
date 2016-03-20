#include <iostream>
#include <sstream>
#include "stdlib.h"

#include "complex_light_beam.h"

using namespace std;

string ComplexLightBeam::str() const
{
  stringstream stream;
  stream << "ComplexLightBeam(" << _r << ", " << _g << ", "  << _b << ")";
  return stream.str();
}

LightBeam abs_beam(const ComplexLightBeam &lb)
{
  return LightBeam(abs(lb.red()), abs(lb.green()), abs(lb.blue()), lb.depth());
}

ComplexLightBeam operator*(const LightBeam &lb, complex<double> x)
{
  return ComplexLightBeam(lb.red()*x, lb.green()*x, lb.blue()*x, lb.depth());
}

