//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "light_beam.h"

#include <stdlib.h>

#include <iostream>
#include <sstream>

using namespace std;

void LightBeam::absorb(const Filter &filter, double dist)
{
  _r *= exp( - filter.red() * dist);
  _g *= exp( - filter.green() * dist);
  _b *= exp( - filter.blue() * dist);
}

string LightBeam::str() const
{
  stringstream stream;
  stream << "LightBeam(" << _r << ", " << _g << ", "  << _b << ")";
  return stream.str();
}

istream& operator>>(istream &in, LightBeam &lb)
{
  string command;
  Color color;
  double lum;

  in >> command;
  parser_assert_command(command, "color");
  in >> color;
  in >> command;
  parser_assert_command(command, "lux");
  in >> lum;

  lb = LightBeam(color, lum);

  return in;
}

