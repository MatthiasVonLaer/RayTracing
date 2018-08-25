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

#include "composition.h"

#include "scene.h"
#include "utilities.h"

#include <iostream>

using namespace std;

Composition::Composition(Composition *parent) :
  Shape(parent),
  _parent(parent),
  _scale(1.)
{
  set_visible(false);
}

void Composition::parse(const string &command, istream &in)
{
  if(command == "load") {
    in >> _filename;
    ifstream input((_path+_filename).c_str());

    if(!input.is_open()) {
      display_error("Composition: Can't find file " + _path + _filename);
    }

    string line;
    while(getline(input, line)) {
      line = line.substr(0,  line.find("//") );
      if(line.size()) {
        _scene->parse(line, this);
      }
    }
  }
  else if(command == "scale") {
    in >> _scale;
  }
  else {
    Shape::parse(command, in);
  }
}

void Composition::setup(Scene *scene, const string &path)
{
  _scene = scene;
  _path = path;
}

std::optional<Plane> Composition::intersect(const Ray &ray) const
{
  throw std::logic_error("Composition is invisible");
}

bool Composition::inside(const Ray &ray) const
{
  throw std::logic_error("Composition is invisible");
}

Color Composition::get_color(const Vector &Vector) const
{
  throw std::logic_error("Composition is invisible");
}

string Composition::name() const
{
  string str;
  if(_parent) {
    str += _parent->name();
  }
  str += _filename;
  return str;
}
