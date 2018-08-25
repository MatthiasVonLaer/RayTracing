//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ray_diagram.h"

#include "ball.h"
#include "camera.h"
#include "cube.h"
#include "function_plot.h"
#include "plane_shape.h"
#include "utilities.h"

using namespace std;

static QColor color_of_shape(const Shape* shape)
{
  if(dynamic_cast<const Ball*>(shape)) {
    return QColor(255, 0, 0);
  }
  else if(dynamic_cast<const Cube*>(shape)) {
    return QColor(0, 0, 255);
  }
  else if(dynamic_cast<const FunctionPlot*>(shape)) {
    return QColor(255, 255, 0);
  }
  else if(dynamic_cast<const PlaneShape*>(shape)) {
    return QColor(0, 255, 255);
  }
  else {
    return QColor(255, 255, 255);
  }
}

RayDiagram::RayDiagram(const Scene &scene, const Camera &camera) :
  _scene(scene),
  _camera(camera),
  _width(300),
  _height(800),
  _y0(-1),
  _number_rays(20),
  _range(10),
  _enabled(false),
  _output_file("ray_diagram.jpg")
{
}

void RayDiagram::parse(const string &command, istream &stream)
{
  _enabled = true;

  if(command == "height") {
    stream >> _height;
  }
  else if(command == "rays") {
    stream >> _number_rays;
  }
  else if(command == "output_file") {
    stream >> _output_file;
  }
  else if(command == "range") {
    stream >> _range;
  }
  else if(command == "width") {
    stream >> _width;
  }
  else if(command == "y0") {
    stream >> _y0;
  }
  else {
    parser_error_unknown_command(command);
  }
}

void RayDiagram::init()
{
  if(!_enabled)
    throw std::runtime_error("RayDiagram disabled.");

  double ds = _range / _height;
  _transformation_matrix = Matrix( ds*(_camera.viewing_direction() ^ _camera.top_direction()),
                                  -ds*_camera.viewing_direction(),
                                   ds*_camera.top_direction()       ).inv();

  if(_y0 < 0) {
    _y0 = _camera.resolution_y()/2;
  }

  _image = QImage(_width, _height, QImage::Format_RGB32);
  _painter.begin(&_image);
}

void RayDiagram::track()
{
  if(!_enabled)
    throw std::runtime_error("RayDiagram disabled.");

  const vector<TrackingData> &data = _scene.tracking_data();

  //track rays
  _scene.clear_tracking();
  _scene.start_tracking();
  for(int i=0; i<_camera.resolution_x(); i+=_camera.resolution_x() / (_number_rays-1)) {
    Vector v = _camera.film_top_left_direction() + _y0*_camera.film_dy() + i*_camera.film_dx();
    _scene.raytracer(Ray(_camera.position(), v));
  }
  _scene.stop_tracking();

  for(const auto& element : data) {
    paint_ray(element.ray, element.distance, element.ratio);
  }

  //track shapes
  _scene.clear_tracking();
  _scene.start_tracking();
  for(int i=0; i<_camera.resolution_x(); i++) {
    Vector v = _camera.film_top_left_direction() + _y0*_camera.film_dy() + i*_camera.film_dx();
    _scene.raytracer(Ray(_camera.position(), v));
  }
  _scene.stop_tracking();

  for(const auto& element : data) {
    paint_shape(element.ray, element.shape, element.distance);
  }
}

void RayDiagram::paint_ray(const Ray &ray, double distance, double ratio)
{
  Vector origin =       _transformation_matrix * (ray.origin() - _camera.position());
  Vector intersection = _transformation_matrix * (ray.origin() + distance * ray.direction() - _camera.position());
  origin       += Vector(_width/2, _height, 0);
  intersection += Vector(_width/2, _height, 0);

  _painter.setPen(QPen(QColor(0, 200*ratio+55, 0), 2));
  _painter.drawLine(origin.x(), origin.y(), intersection.x(), intersection.y());
}

void RayDiagram::paint_shape(const Ray &ray, const Shape *shape, double distance)
{
  Vector origin =       _transformation_matrix * (ray.origin() - _camera.position());
  Vector intersection = _transformation_matrix * (ray.origin() + distance * ray.direction() - _camera.position());
  origin       += Vector(_width/2, _height, 0);
  intersection += Vector(_width/2, _height, 0);

  _painter.setPen(QPen(color_of_shape(shape), 2));
  _painter.drawPoint(intersection.x(), intersection.y());
}

void RayDiagram::save() const
{
  if(!_enabled)
    throw std::runtime_error("RayDiagram disabled.");

  _image.save(_output_file.c_str(), 0, 100);
}
