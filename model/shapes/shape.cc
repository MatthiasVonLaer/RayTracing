//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "shape.h"

#include "composition.h"
#include "utilities.h"

#include <iostream>

using namespace std;

Shape::Shape(Composition *parent):
  _parent(parent),
  _color_type (OPAQUE),
  _shape_type (SOLID),
  _color      (Color(255, 255, 255)),
  _silvered   (0),
  _visible    (true),
  _regular    (false),
  _width(1.),
  _depth(1.),
  _height(1.),
  _width_parsed(false),
  _depth_parsed(false),
  _height_parsed(false),
  _top_direction(0, 0, 1),
  _front_direction(0, -1, 0)
{
}

void Shape::parse(const string &command, istream &in)
{
  if(command == "color") {
    in >> _color;
    _color_type = OPAQUE;
  }
  else if(command == "depth") {
    in >> _depth;
    _depth_parsed = true;
  }
  else if(command == "front_direction") {
    in >> _front_direction;
  }
  else if(command == "height") {
    in >> _height;
    _height_parsed = true;
  }
  else if(command == "length") {
    double d;
    in >> d;
    _width = _depth = _height = d;
    _width_parsed = _depth_parsed = _height_parsed = true;
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
    if(_shape_type == SURFACE) {
      display_error("Surface may not be transparent.");
    }
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
    in >> _width;
    _width_parsed = true;
  }
  else {
    parser_error_unknown_command(command);
  }

}

void Shape::init_sequence()
{
  _dx     = this->init_dx();
  _dy     = this->init_dy();
  _dz     = this->init_dz();

  init_local_coordinates();

  _origin = this->init_origin();
  if(_parent) {
    _origin = _parent->transformation_matrix() * _origin + _parent->origin();
  }

  this->init_derived_class();
}

void Shape::init_local_coordinates()
{
  _front_direction.normalize();
  _top_direction -= (_front_direction * _top_direction) * _front_direction;
  _top_direction.normalize();

  _vx = _dx * (_top_direction ^ _front_direction);
  _vy = -_dy * _front_direction;
  _vz = _dz * _top_direction;

  if(_parent) {
    _parent->init_sequence();
    _vx = _parent->transformation_matrix() * _vx;
    _vy = _parent->transformation_matrix() * _vy;
    _vz = _parent->transformation_matrix() * _vz;
  }

  _T_local_to_global = Matrix( _vx, _vy, _vz);

  if(is_equal(0, _T_local_to_global.det())) {
    if(visible()) {
      display_warning("shape at " + _position.str() + " not regular.");
    }
    _regular = false;
  }
  else {
    _T_global_to_local = _T_local_to_global.inv();
    _regular = true;
  }
}

Vector Shape::global_to_local_point(const Vector &v) const
{
  return _T_global_to_local * (v - _origin);
}

Vector Shape::global_to_local_direction(const Vector &v) const
{
  return _T_global_to_local * v;
}

Vector Shape::local_to_global_point(const Vector &v) const
{
  return (_T_local_to_global * v) + _origin;
}

Vector Shape::local_to_global_direction(const Vector &v) const
{
  return _T_local_to_global * v;
}
