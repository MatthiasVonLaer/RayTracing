#pragma once

#include <string>

#include "color.h"
#include "filter.h"
#include "vector.h"

class LightBeam
{
private:
  double _r;
  double _g;
  double _b;
  double _depth;

public:
  LightBeam()                                         {_r=0; _g=0; _b=0; _depth=0;}
  LightBeam(double r, double g, double b, double d=0) {_r=r; _g=g; _b=b; _depth=d;}
  LightBeam(const Color &c, double lum)               {_r=c.red()*lum/255; _g=c.green()*lum/255; _b=c.blue()*lum/255; _depth=0;}

  void absorb(const Filter &filter, double dist);

  void      operator+= (const LightBeam &lb)          {_r+=lb.red(); _g+=lb.green(); _b+=lb.blue(); _depth+=lb.depth();}
  void      operator*= (double d)                     { _r*=d; _g*=d; _b*=d; _depth*=d;}
  void      operator/= (double d)                     { _r/=d; _g/=d; _b/=d; _depth/=d;}
  LightBeam operator-  (const Color &c) const         {return LightBeam(_r*c.red()/255, _g*c.green()/255, _b*c.blue()/255);}
  LightBeam operator+  (const LightBeam &lb) const    {return LightBeam(_r+lb.red(), _g+lb.green(), _b+lb.blue(), _depth+lb.depth());}
  LightBeam operator*  (double d) const               {return LightBeam(_r*d, _g*d, _b*d, _depth*d);}
  LightBeam operator^  (double d) const               {return LightBeam(pow(_r, d), pow(_g, d), pow(_b, d), _depth);}
  LightBeam operator/  (double d) const               {return LightBeam(_r/d, _g/d, _b/d, _depth/d);}

  double red() const                                  {return _r;}
  double green() const                                {return _g;}
  double blue() const                                 {return _b;}
  double luminosity() const                           {return std::max(_r, std::max(_g, _b));}
  double depth() const                                {return _depth;}

  void set_depth(double d)                            {_depth = d;}

  std::string str() const;
};

std::istream& operator>>(std::istream &in, LightBeam &lb);
