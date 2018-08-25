//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "camera.h"
#include "ray_diagram.h"
#include "scene.h"

#include <istream>

class Controller
{
public:
  void parse(std::istream &in);
  Camera& camera() { return _camera; }
  Scene& scene() { return _scene; }

private:
  void initialize();
  void take_picture();
  void launch_gui();

  Scene _scene;
  Display _display;
  Camera _camera{_scene, _display};
  RayDiagram _ray_diagram{_scene, _camera};
};
