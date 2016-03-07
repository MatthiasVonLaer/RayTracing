#include <sstream>

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

void RayDiagram::parse(const string &line)
{
  _enabled = true;

  stringstream stream(line);
  string command;

  while(stream >> command) {
    if(command == "disable") {
      _enabled = false;
    }
    else if(command == "enable") {
      _enabled = true;
    }
    else if(command == "height") {
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
}

void RayDiagram::init(const Vector &position, const Vector &viewing_direction, const Vector &top_direction, int res_x, int res_y)
{
  if(mpi.size() > 1) {
    display_warning("RayDiagram runs only in serial mode.");
  }

  if(!_enabled)
    display_error("RayDiagram disabled.");

  _position = position;
  double ds = _range / _height;
  _transformation_matrix = Matrix( ds*(viewing_direction ^ top_direction), -ds*viewing_direction, ds*top_direction ).inv();
  _resolution_x = res_x;
  if(_y0 < 0) {
    _y0 = res_y/2;
  }
  
  _image = QImage(_width, _height, QImage::Format_RGB32);
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
