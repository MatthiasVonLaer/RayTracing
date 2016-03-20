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

#include <iostream>
#include <sstream>
#include "stdlib.h"

#include "light_beam.h"

using namespace std;

void LightBeam::absorb(const Filter &filter, double dist)
{
  _r *= exp( - filter.red() * dist);
  _g *= exp( - filter.green() * dist);
  _b *= exp( - filter.blue() * dist);
}

string LightBeam::str() const
{
  stringstream stream;
  stream << "LightBeam(" << _r << ", " << _g << ", "  << _b << ")";
  return stream.str();
}

istream& operator>>(istream &in, LightBeam &lb)
{
  string command;
  Color color;
  double lum;

  in >> command;
  parser_assert_command(command, "color");
  in >> color;
  in >> command;
  parser_assert_command(command, "lux");
  in >> lum;

  lb = LightBeam(color, lum);

  return in;
}

