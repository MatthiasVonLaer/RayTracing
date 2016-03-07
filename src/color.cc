#include <iostream>
#include <sstream>

#include "color.h"

using namespace std;

std::string Color::str()
{
  stringstream stream;
  stream << "Color(" << _r << ", " << _g << ", " << _b << ")" << endl;
  return stream.str();
}

istream& operator>>(istream &in, Color &color)
{
  int a, b, c;
  in >> a >> b >> c;
  color = Color(a, b, c);
  return in;
}
