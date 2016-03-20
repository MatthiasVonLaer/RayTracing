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

