//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "ray.h"
#include "shapes/shape.h"

class TrackingData
{
public:
  Ray ray;
  const Shape *shape;
  double distance;
  double ratio;

public:
  TrackingData(const Ray &ray_in, const Shape *shape_in, double distance_in, double ratio_in) :
    ray(ray_in), shape(shape_in), distance(distance_in), ratio(ratio_in) {}
};
