//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
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
