#include <sstream>

#include "camera.h"
#include "mpi_manager.h"
#include "ray_diagram.h"
#include "utilities.h"

using namespace std;

RayDiagram::RayDiagram() :
  _width(300),
  _height(800),
  _y0(-1),
  _number_rays(20),
  _range(10),
  _enabled(false),
  _painter(0),
  _output_file("ray_diagram.jpg")
{
}

RayDiagram::~RayDiagram()
{
  delete _painter;
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

void RayDiagram::init(const Camera &camera)
{
  if(mpi.size() > 1) {
    display_warning("RayDiagram runs only in serial mode.");
  }

  if(!_enabled)
    display_error("RayDiagram disabled.");

  _position = camera.position();
  double ds = _range / _height;
  _transformation_matrix = Matrix( ds*(camera.viewing_direction() ^ camera.top_direction()),
                                  -ds*camera.viewing_direction(),
                                   ds*camera.top_direction() ).inv();
  _resolution_x = camera.resolution_x();
  if(_y0 < 0) {
    _y0 = camera.resolution_y()/2;
  }
  
  _image = QImage(_width, _height, QImage::Format_RGB32);
  delete _painter;
  _painter = new QPainter(&_image);
}

void RayDiagram::add(const Ray &ray, double distance, double ratio)
{
  if(!_enabled)
    display_error("RayDiagram disabled.");

  if(_pixel_in_progress_y == _y0) {
    Vector origin =       _transformation_matrix * (ray.origin() - _position);
    Vector intersection = _transformation_matrix * (ray.origin() + distance * ray.direction() - _position);
    origin       += Vector(_width/2, _height, 0);
    intersection += Vector(_width/2, _height, 0);

    _painter->setPen(QPen(QColor(255, 0, 0), 2));
    _painter->drawPoint(intersection.x(), intersection.y());

    if(_pixel_in_progress_x % (_resolution_x / (_number_rays-1)) == 0) {
      _painter->setPen(QPen(QColor(0, 200*ratio+55, 0), 2));
      _painter->drawLine(origin.x(), origin.y(), intersection.x(), intersection.y());
    }
  }

}

void RayDiagram::set_pixel_in_progress(int x, int y)
{
  if(!_enabled)
    display_error("RayDiagram disabled.");

  _pixel_in_progress_x = x;
  _pixel_in_progress_y = y;
}

void RayDiagram::save() const
{
  if(!_enabled)
    display_error("RayDiagram disabled.");

  _image.save(_output_file.c_str(), 0, 100);
}
