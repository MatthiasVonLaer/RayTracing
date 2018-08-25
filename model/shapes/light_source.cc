//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
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
