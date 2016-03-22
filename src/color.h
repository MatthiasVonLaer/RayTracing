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

#pragma once

#include <istream>
#include <string>

#include <QColor>

class Color
{
private:
  int _r;
  int _g;
  int _b;

public:
  Color()                                 {_r=0; _g=0; _b=0;}
  Color(int r_in, int g_in, int b_in)     {_r=r_in; _g=g_in; _b=b_in;}
  Color(const QRgb &color)                {_r=qRed(color); _g=qGreen(color); _b=qBlue(color);}

  int red() const                         {return _r;}
  int green() const                       {return _g;}
  int blue() const                        {return _b;}

  std::string str();
};

std::istream& operator>>(std::istream &in, Color &color);
