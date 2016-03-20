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
