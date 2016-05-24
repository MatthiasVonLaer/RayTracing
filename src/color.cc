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

#include "color.h"

#include <sstream>

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
