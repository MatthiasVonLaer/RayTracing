//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "mpi_manager.h"
#include "scene.h"
#include "vector.h"

class Slave
{
private:
  Scene _scene;

  int    _camera_resolution_x;
  Vector _camera_position;
  Vector _camera_film_top_left_direction;
  Vector _camera_film_dx;
  Vector _camera_film_dy;

  bool _idle_sleep;

public:
  Slave();
  void loop();

private:
  void parse();
  void init();
  void camera_data();
  void raytrace();
  void set_idle();
};
