#include "light_source.h"

using namespace std;

LightSource::LightSource()
{
  _radius = 0;
  _invisible = true;
}

void LightSource::parse(const string &command, istream &in)
{
  Color c;
  double lum;

  if(command == "radius") {
    in >> _radius;
    _invisible = false;
  }
  else if(command == "color") {
    in >> c;
    string command2;
    in >> command2;
    parser_assert_command(command2, "lumen");
    in >> lum;
    _beam = LightBeam(c, lum);
  }
  else {
    Ball::parse(command, in);
  }
}
