//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "color.h"
#include "light_beam.h"
#include "vector.h"

#include <complex>
#include <string>

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

  ComplexLightBeam(const LightBeam &lb)                       {_r=lb.red(); _g=lb.green(); _b=lb.blue();
                                                               _depth=lb.depth();}

  ComplexLightBeam& operator+= (const ComplexLightBeam &lb)   {_r+=lb.red(); _g+=lb.green(); _b+=lb.blue();
                                                               _depth+=lb.depth(); return *this;}
  ComplexLightBeam& operator*= (double d)                     { _r*=d; _g*=d; _b*=d; _depth*=d; return *this;}
  ComplexLightBeam& operator/= (double d)                     { _r/=d; _g/=d; _b/=d; _depth/=d; return *this;}

  std::complex<double> red() const                            {return _r;}
  std::complex<double> green() const                          {return _g;}
  std::complex<double> blue() const                           {return _b;}
  double luminosity() const                                   {return std::max(abs(_r), std::max(abs(_g), abs(_b)));}
  double depth() const                                        {return _depth;}

  std::string str() const;
};

LightBeam abs_beam(const ComplexLightBeam &lb);
const ComplexLightBeam operator*(const LightBeam &lb, std::complex<double> x);
