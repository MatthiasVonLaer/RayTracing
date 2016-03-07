#pragma once

#include <math.h>

#include "color.h"

class Filter
{
public:
  Filter()                                        {_r=1; _g=1; _b=1;}
  Filter(double r_in, double g_in, double b_in)   {_r=r_in; _g=g_in; _b=b_in;}
  Filter(const Color &color)                      {_r = color.red() / 255.; _g = color.green() / 255.; _b = color.blue() / 255.;}

  //void set_red(double d)                          {_r = d;}
  //void set_green(double d)                        {_g = d;}
  //void set_blue(double d)                         {_b = d;}

  double red() const                              {return _r;}
  double green() const                            {return _g;}
  double blue() const                             {return _b;}

  Filter operator^ (double d) const               {return Filter(pow(_r, d), pow(_g, d), pow(_b, d));}
  void   operator*=(const Filter &filter)         {_r *= filter.red(); _g *= filter.green(); _b *= filter.blue();}

private:
  double _r;
  double _g;
  double _b;
};

std::istream& operator>>(std::istream &in, Filter &filter);
