#include "light_source.h"

using namespace std;

LightSource::LightSource(const Composition *parent) :
  Ball(parent)
{
  _radius = 0;
  _visible = false;
}

void LightSource::parse(const string &command, istream &in)
{
  Color c;
  double lum;

  if(command == "radius") {
    in >> _radius;
    _visible = true;
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
