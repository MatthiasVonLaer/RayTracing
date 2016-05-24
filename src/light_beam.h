//  Copyright 2016 Matthias von Laer
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "color.h"
#include "filter.h"
#include "vector.h"

#include <string>

class LightBeam
{
private:
  double _r;
  double _g;
  double _b;
  double _depth;

public:
  LightBeam()                                               {_r=0; _g=0; _b=0; _depth=0;}
  LightBeam(double r, double g, double b, double d=0)       {_r=r; _g=g; _b=b; _depth=d;}
  LightBeam(const Color &c, double lum)                     {_r=c.red()*lum/255; _g=c.green()*lum/255;
                                                             _b=c.blue()*lum/255; _depth=0;}

  void absorb(const Filter &filter, double dist);

  LightBeam& operator+= (const LightBeam &lb)               {_r+=lb.red(); _g+=lb.green(); _b+=lb.blue();
                                                             _depth+=lb.depth(); return *this;}
  LightBeam& operator*= (double d)                          { _r*=d; _g*=d; _b*=d; _depth*=d; return *this;}
  LightBeam& operator/= (double d)                          { _r/=d; _g/=d; _b/=d; _depth/=d; return *this;}

  const LightBeam operator-  (const Color &c) const         {return LightBeam(_r*c.red()/255, _g*c.green()/255,
                                                                              _b*c.blue()/255);}

  const LightBeam operator+  (const LightBeam &lb) const    {return LightBeam(_r+lb.red(), _g+lb.green(),
                                                                              _b+lb.blue(), _depth+lb.depth());}

  const LightBeam operator*  (double d) const               {return LightBeam(_r*d, _g*d, _b*d, _depth*d);}

  const LightBeam operator^  (double d) const               {return LightBeam(pow(_r, d), pow(_g, d), pow(_b, d),
                                                                              _depth);}

  const LightBeam operator/  (double d) const               {return LightBeam(_r/d, _g/d, _b/d, _depth/d);}

  double red() const                                        {return _r;}
  double green() const                                      {return _g;}
  double blue() const                                       {return _b;}
  double luminosity() const                                 {return std::max(_r, std::max(_g, _b));}
  double depth() const                                      {return _depth;}

  void set_depth(double d)                                  {_depth = d;}

  std::string str() const;
};

std::istream& operator>>(std::istream &in, LightBeam &lb);
