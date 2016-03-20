#include <iostream>

#include "composition.h"
#include "scene.h"
#include "utilities.h"

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

void Composition::setup(Scene *scene, const Composition *parent, const string &path)
{
  _scene = scene;
  _parent = parent;
  _path = path;
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
