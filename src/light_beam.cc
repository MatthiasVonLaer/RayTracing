#include <iostream>
#include <sstream>
#include "stdlib.h"

#include "light_beam.h"

using namespace std;

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
  parser_assert_command(command, "lumen");
  in >> lum;

  lb = LightBeam(color, lum);

  return in;
}

