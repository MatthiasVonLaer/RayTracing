#pragma once

#include <math.h>

#include "color.h"

class Filter
{
private:
  double _r;
  double _g;
  double _b;

public:
  Filter()                                        {_r=1; _g=1; _b=1;}
  Filter(double r_in, double g_in, double b_in)   {_r=r_in; _g=g_in; _b=b_in;}
  Filter(const Color &color)                      {_r = color.red() / 255.; _g = color.green() / 255.; _b = color.blue() / 255.;}

  double absorption(double distance) const        {return exp(-distance * std::min(_r, std::min(_g, _b)) );}

  double red() const                              {return _r;}
  double green() const                            {return _g;}
  double blue() const                             {return _b;}
};

std::istream& operator>>(std::istream &in, Filter &filter);
