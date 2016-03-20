#pragma once

#include <complex>
#include <string>

#include "color.h"
#include "light_beam.h"
#include "vector.h"

class ComplexLightBeam
{
private:
  std::complex<double> _r;
  std::complex<double> _g;
  std::complex<double> _b;
  double _depth;

public:
  ComplexLightBeam()                                          {_r=0; _g=0; _b=0; _depth=0;}

  ComplexLightBeam(std::complex<double> r,
                   std::complex<double> g,
                   std::complex<double> b, double d=0)        {_r=r; _g=g; _b=b; _depth=d;}

  ComplexLightBeam(const LightBeam &lb)                       {_r=lb.red(); _g=lb.green(); _b=lb.blue(); _depth=lb.depth();}

  void      operator+= (const ComplexLightBeam &lb)           {_r+=lb.red(); _g+=lb.green(); _b+=lb.blue(); _depth+=lb.depth();}
  void      operator*= (double d)                             { _r*=d; _g*=d; _b*=d; _depth*=d;}
  void      operator/= (double d)                             { _r/=d; _g/=d; _b/=d; _depth/=d;}

  std::complex<double> red() const                            {return _r;}
  std::complex<double> green() const                          {return _g;}
  std::complex<double> blue() const                           {return _b;}
  double luminosity() const                                   {return std::max(abs(_r), std::max(abs(_g), abs(_b)));}
  double depth() const                                        {return _depth;}

  std::string str() const;
};

LightBeam abs_beam(const ComplexLightBeam &lb);
ComplexLightBeam operator*(const LightBeam &lb, std::complex<double> x);
