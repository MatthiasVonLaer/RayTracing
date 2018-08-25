//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "camera.h"
#include "matrix.h"
#include "ray.h"
#include "scene.h"
#include "shapes/shape.h"

#include <QImage>
#include <QPainter>

#include <string>

class RayDiagram
{
private:
  const Scene &_scene;
  const Camera &_camera;

  QImage _image;
  QPainter _painter;

  Matrix _transformation_matrix;

  double _range;
  int _width;
  int _height;
  int _y0;
  int _number_rays;

  std::string _output_file;

  bool _enabled;

public:
  RayDiagram(const Scene &scene, const Camera &camera);

  void parse(const std::string &command, std::istream &stream);
  void init();

public:
  void track();
private:
  void paint_ray(const Ray &ray, double distance, double ratio);
  void paint_shape(const Ray &ray, const Shape *shape, double distance);

public:
  void save() const;

  bool enabled() const  {return _enabled;}
};
