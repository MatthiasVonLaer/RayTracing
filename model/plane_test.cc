//  A raytracer program for rendering photorealistic images
//  Copyright 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "plane.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( Raytracing_Plane_distance )
{
  const Plane plane({0, 0, 1}, {0, 0, 1});
  BOOST_CHECK_EQUAL(1, distance(plane, Vector()));
}
