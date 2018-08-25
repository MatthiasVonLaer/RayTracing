//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "filter.h"

using namespace std;

istream& operator>>(istream &in, Filter &filter)
{
  double r, g, b;
  in >> r >> g >> b;
  filter = Filter(r, g, b);
  return in;
}
