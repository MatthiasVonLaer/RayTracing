#include <iostream>
#include "shape.h"
#include "utilities.h"

using namespace std;

Shape::Shape():
  _color_type (OPAQUE),
  _shape_type (SOLID),
  _color      (Color(255, 255, 255)),
  _silvered   (0),
  _invisible  (false),
  _dx(1.),
  _dy(1.),
  _dz(1.),
  _top_direction(0, 0, 1),
  _front_direction(0, -1, 0),
  _init_unit_vectors(false),
  _init_transformation_matrix(false)
{
}

void Shape::init_unit_vectors()
{
  _front_direction.normalize();
  _top_direction -= (_front_direction * _top_direction) * _front_direction;
  _top_direction.normalize();
  _init_unit_vectors = true;
}

void Shape::init_transformation_matrix()
{
  _T_local_to_global = Matrix( _dx * vx(),
                               _dy * vy(),
                               _dz * vz());

  if(is_equal(0, _T_local_to_global.det())) {
    display_warning("shape at " + _position.str() + " not regular.");
    _invisible = true;
  }
  else {
    _T_global_to_local = _T_local_to_global.inv();
    _init_transformation_matrix = true;
  }
}

void Shape::parse(const string &command, istream &in)
{
  if(command == "color") {
    in >> _color;
    _color_type = OPAQUE;
  }
  if(command == "depth") {
    in >> _dy;
    _dy /= 2;
  }
  else if(command == "front_direction") {
    in >> _front_direction;
  }
  else if(command == "height") {
    in >> _dz;
    _dz /= 2;
  }
  else if(command == "length") {
    double d;
    in >> d;
    _dx = _dy = _dz = d/2;
  }
  else if(command == "position") {
    in >> _position;
  }
  else if(command == "silvered") {
    in >> _silvered;
  }
  else if(command == "top_direction") {
    in >> _top_direction;
  }
  else if(command == "transparent") {
    string cmd;
    in >> cmd;
    parser_assert_command(cmd, "color_filter");

    in >> _filter;

    in >> cmd;
    parser_assert_command(cmd, "refraction_index");
    in >> _refraction_index;

    _color_type = TRANSPARENT;
  }
  else if(command == "width") {
    in >> _dx;
    _dx /= 2;
  }
  else {
    parser_error_unknown_command(command);
  }

}

void Shape::load_image(const string &path, QImage* &im) const
{
  if(im) {
    delete im;
  }
  im = new QImage(path.c_str());
  if(im->isNull()) {
    display_warning("Couldn't load image " + path);
    delete im;
    im = 0;
  }
}

Vector Shape::global_to_local_point(const Vector &v) const
{
  assert(_init_transformation_matrix);
  return _T_global_to_local * (v - _position);
}

Vector Shape::global_to_local_direction(const Vector &v) const
{
  assert(_init_transformation_matrix);
  return _T_global_to_local * v;
}

Vector Shape::local_to_global_point(const Vector &v) const
{
  assert(_init_transformation_matrix);
  return (_T_local_to_global * v) + _position;
}

Vector Shape::local_to_global_direction(const Vector &v) const
{
  assert(_init_transformation_matrix);
  return _T_local_to_global * v;
}