#include "composition.h"
#include "utilities.h"

using namespace std;

Composition::Composition(const Composition *parent) :
  Shape(parent)
{
  set_visibility(false);
}

void Composition::parse(const string &command, istream &in)
{
  if(command == "load") {
    in >> _path;
    _input.open(_path.c_str());

    if(!_input.is_open()) {
      display_error("Composition: Can't find file " + _path);
    }
  }
  else {
    Shape::parse(command, in);
  }
}
