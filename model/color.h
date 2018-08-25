//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QColor>

#include <istream>

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
