//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ray.h"

#include "utilities.h"

using namespace std;

bool Ray::intersect(const Plane &plane, double &intersection_distance) const
{
  double ray_normal_part = plane.normal() * direction();

  if(is_equal(0, ray_normal_part))
    return false;

  double distance_ray_plane = plane.normal() * (plane.origin() - origin());
  intersection_distance  = distance_ray_plane / ray_normal_part;
	
  if(is_greater(intersection_distance, 0)) {
    return true;
  }
  else {
    return false;
  }
}
