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

class Filter
{
private:
  double _r;
  double _g;
  double _b;

public:
  Filter()                                        {_r=1; _g=1; _b=1;}
  Filter(double r_in, double g_in, double b_in)   {_r=r_in; _g=g_in; _b=b_in;}
  Filter(const Color &color)                      {_r = color.red() / 255.; _g = color.green() / 255.;
                                                   _b = color.blue() / 255.;}

  double absorption(double distance) const        {return exp(-distance * std::min(_r, std::min(_g, _b)) );}

  double red() const                              {return _r;}
  double green() const                            {return _g;}
  double blue() const                             {return _b;}
};

std::istream& operator>>(std::istream &in, Filter &filter);
