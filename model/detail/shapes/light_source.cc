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

#include "light_source.h"

using namespace std;

LightSource::LightSource(Composition *parent) :
  Ball(parent)
{
  set_radius(0);
  set_visible(false);
}

void LightSource::parse(const string &command, istream &in)
{
  Color c;
  double lum;

  if(command == "radius") {
    double r;
    in >> r;
    set_radius(r);
    set_visible(true);
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
