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

