//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "complex_light_beam.h"

#include <sstream>

using namespace std;

string ComplexLightBeam::str() const
{
  stringstream stream;
  stream << "ComplexLightBeam(" << _r << ", " << _g << ", "  << _b << ")";
  return stream.str();
}

LightBeam abs_beam(const ComplexLightBeam &lb)
{
  return LightBeam(abs(lb.red()), abs(lb.green()), abs(lb.blue()), lb.depth());
}

const ComplexLightBeam operator*(const LightBeam &lb, complex<double> x)
{
  return ComplexLightBeam(lb.red()*x, lb.green()*x, lb.blue()*x, lb.depth());
}

