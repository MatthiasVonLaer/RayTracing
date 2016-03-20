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

#include <iostream>
#include <sstream>
#include <set>
#include <typeinfo>
#include <assert.h>

#include "ball.h"
#include "composition.h"
#include "cube.h"
#include "cylinder.h"
#include "function_plot.h"
#include "plane_shape.h"
#include "scene.h"

using namespace std;

Scene::Scene() :
  _fog(0),
  _recursion_break_ratio(0.1),
  _tracking_active(false)
{
}

Scene::~Scene()
{
  for(int i=0; i<_shapes.size(); i++) {
    delete _shapes[i];
  }
}

void Scene::parse(const string &line, Composition *parent)
{
  stringstream stream(line);
  string command;

  stream >> command;

  if(command == "ambient_light") {
    stream >> _ambient_light;
  }
  else if(command == "compositions_dir") {
    stream >> _compositions_dir;
    _compositions_dir += "/";
  }
  else if(command == "fog") {
    stream >> _fog;
  }
  else if(command == "raytracer") {
    string command2;
    stream >> command2;
    if(command2 == "recursion_break_ratio") {
      stream >> _recursion_break_ratio;
    }
    else if(command2 == "recursion_break_depth") {
      stream >> _recursion_break_depth;
    }
    else {
      parser_error_unknown_command(command);
    }
  }
  else if(command == "remove") {
    string name1, name2;
    stream >> name1 >> name2;
    remove_shape(name1 + "_" + name2);
  }
  else if(command == "sky") {
    stream >> _sky;
  }
  //shapes
  else if(command == "ball") {
    get_or_create_shape_and_parse<Ball>(command, stream, parent);
  }
  else if(command == "composition") {
    get_or_create_shape_and_parse<Composition>(command, stream, parent);
  }
  else if(command == "cube") {
    get_or_create_shape_and_parse<Cube>(command, stream, parent);
  }
  else if(command == "cylinder") {
    get_or_create_shape_and_parse<Cylinder>(command, stream, parent);
  }
  else if(command == "function_plot") {
    get_or_create_shape_and_parse<FunctionPlot>(command, stream, parent);
  }
  else if(command == "light") {
    get_or_create_shape_and_parse<LightSource>(command, stream, parent);
  }
  else if(command == "plane") {
    get_or_create_shape_and_parse<PlaneShape>(command, stream, parent);
  }
  else {
    parser_error_unknown_command(command);
  }

}

template<class T> void Scene::get_or_create_shape_and_parse(const std::string &shape_class, std::istream &in, Composition *parent)
{
  string id;
  in >> id;

  string name;
  if(parent) {
    name += parent->name() + "/";
  }

  name += shape_class + "_" + id;

  Shape *shape;

  if(!_shapes_by_name.count(name)) {
    shape = new T(parent);
    _shapes_by_name[name] = shape;
    _shapes.push_back(shape);
    if(typeid(T) == typeid(LightSource)) {
      _lights.push_back( dynamic_cast<LightSource*>(shape) );
    }
    else if(typeid(T) == typeid(Composition)) {
      _compositions.push_back( dynamic_cast<Composition*>(shape) );
      _compositions.back()->setup(this, parent, _compositions_dir);
    }
  }
  else {
    shape = _shapes_by_name[name];
  }

  string command;
  while(in >> command) {
    shape->parse(command, in);
  }
}

void Scene::remove_shape(const string &name)
{
  if(!_shapes_by_name.count(name)) {
    display_warning("shape to remove " + name + " doesn't exist.");
    return;
  }

  for(int i=0; i<_lights.size(); i++) {
    if(_lights[i] == _shapes_by_name[name]) {
      _lights.erase(_lights.begin() + i);
      return;
    }
  }

  delete _shapes_by_name[name];
  _shapes_by_name.erase(name);
}


void Scene::init()
{
  for(map<string, Shape*>::iterator it = _shapes_by_name.begin(); it != _shapes_by_name.end(); it++) {
    it->second->init_sequence();
  }

  for(int i=_shapes.size()-1; i>=0; i--) {
    if(!_shapes[i]->visible() || !_shapes[i]->regular()) {
      _shapes.erase( _shapes.begin() + i);
    }
  }

}



LightBeam Scene::raytracer(const Ray &ray) const
{
  return raytracer(ray, get_inside_shape(ray), 1, 0);
}

LightBeam Scene::raytracer(const Ray &ray, const Shape *inside_shape, double ratio, int depth) const
{
  //Get intersection point
  Plane intersection_plane;
  const Shape *intersection_shape = get_intersection_shape(ray, inside_shape, intersection_plane);
  double distance = (ray.origin() - intersection_plane.origin()).norm();
  
  //Get Lightbeam
  LightBeam lightbeam;
  if(intersection_shape) {

    if(inside_shape) {
      ratio *= inside_shape->filter().absorption(distance);
    }
    if(ratio < _recursion_break_ratio || depth >= _recursion_break_depth)
      return LightBeam();

    //TRANSPARENT
    if(intersection_shape->color_type() == TRANSPARENT) {
      const Shape *inside_shape_2 = get_inside_shape(Ray(intersection_plane.origin(), ray.direction() ));
      if(!inside_shape_2 || inside_shape_2->color_type() == TRANSPARENT) {
        lightbeam = refraction(ray, intersection_plane, inside_shape, inside_shape_2, intersection_shape, ratio, depth+1);
      }
      else {
        lightbeam = scattered_light(intersection_plane, inside_shape_2->get_color(intersection_plane.origin())) * (1 - intersection_shape->silvered());
      }
    }

    //OPAQUE
    else {
      if(is_greater(ratio * intersection_shape->silvered(), _recursion_break_ratio)) {
        lightbeam += reflection(ray, intersection_plane, inside_shape, ratio * intersection_shape->silvered(), depth+1) * intersection_shape->silvered();
      }
      if(is_greater(ratio * (1-intersection_shape->silvered()), _recursion_break_ratio)) {
        lightbeam += scattered_light(intersection_plane, intersection_shape->get_color(intersection_plane.origin())) * (1 - intersection_shape->silvered());
      }
    }

    //FILTER
    if(inside_shape) {
      lightbeam.absorb(inside_shape->filter(), distance);
    }
    //FOG
    else if(is_greater(_fog, 0)) {
      double fog_factor = exp(-_fog * distance);
      lightbeam = lightbeam*fog_factor + _ambient_light*(1-fog_factor);
    }
  }
  //No intersection
  else {
    distance = 1e8;
    if(is_equal(_fog, 0))
      lightbeam = _sky;
    else
      lightbeam = _ambient_light;
  }

  if(_tracking_active) {
    _tracking_data.push_back( TrackingData(ray, intersection_shape, distance, ratio) );
  }

  lightbeam.set_depth(distance);

  return lightbeam;
}

LightBeam Scene::reflection(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, double ratio, int depth) const
{
  Vector p = intersection_plane.origin();
  Vector u = ray.direction() - intersection_plane.normal() * (2 * (ray.direction() * intersection_plane.normal()));

  return raytracer(Ray(p, u), inside_shape, ratio, depth);
}

LightBeam Scene::refraction(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, const Shape *inside_shape_2,
                            const Shape *intersection_shape, double ratio, int depth) const
{
  Vector normal = intersection_plane.normal();
  if(is_greater( 0, ray.direction()*normal ))
    normal = -normal;
	
  double n1=1;
  double n2=1;
  if(inside_shape)
    n1 = inside_shape->refraction_index();
  if(inside_shape_2)
    n2 = inside_shape_2->refraction_index();
  double n = n2/n1;
	
  Vector tangential = (ray.direction() - normal * (ray.direction() * normal)) / n;
  double refKoeff;
  if(is_greater( 1, tangential.norm2() )) {
    Vector p = intersection_plane.origin();
    Vector u = tangential + normal * sqrt(1 - tangential.norm2());
    Ray refraction_ray(p, u);

    //Schlick's approximation
    double R_0 = pow( (n1 - n2) / (n1 + n2), 2);
    R_0 += (1-R_0) * intersection_shape->silvered();
    double reflection_coefficient;
    if(n > 1)
      reflection_coefficient = R_0 + (1 - R_0) * pow(1 - ray.direction()*normal, 5);
    else
      reflection_coefficient = R_0 + (1 - R_0) * pow(1 - refraction_ray.direction()*normal, 5);

    LightBeam lightbeam;
    if(is_greater(ratio*reflection_coefficient, _recursion_break_ratio)) {
      lightbeam += reflection(ray, intersection_plane, inside_shape, ratio*reflection_coefficient, depth) * reflection_coefficient;
    }
    if(is_greater(ratio*(1-reflection_coefficient), _recursion_break_ratio)) {
      lightbeam += raytracer(refraction_ray, inside_shape_2, ratio*(1-reflection_coefficient), depth) * (1-reflection_coefficient);
    }
    return lightbeam;
  }
  else
    return reflection(ray, intersection_plane, inside_shape, ratio, depth);
}

LightBeam Scene::scattered_light(const Plane &intersection_plane, const Color &color) const
{
  LightBeam light_beam = _ambient_light;

  for(int i=0; i<_lights.size(); i++) {
    Ray light_ray(intersection_plane.origin(),
                  _lights[i]->origin() - intersection_plane.origin());

    double distance = (_lights[i]->origin() - intersection_plane.origin()).norm();

    double fog_factor = exp(-_fog * distance);

    light_beam += light_beam_from_source(light_ray, _lights[i], distance)
                  * (fabs(light_ray.direction() * intersection_plane.normal()) / (4*PI*distance*distance)
                  * fog_factor);
  }
	
  return light_beam - color;
}

LightBeam Scene::light_beam_from_source(const Ray &ray, const LightSource *light, double distance_light) const
{
  const Shape *inside_shape = get_inside_shape(ray);
  Plane intersection_plane;
  const Shape *intersection_shape = get_intersection_shape(ray, inside_shape, intersection_plane);
  double distance = (intersection_plane.origin() - ray.origin()).norm();
  if(!intersection_shape || light == intersection_shape || is_greater(distance, distance_light)) {
    return light->beam();
  }
  else if(intersection_shape->color_type() != TRANSPARENT || (inside_shape && inside_shape->color_type() != TRANSPARENT)) {
    return LightBeam(); 
  }
  else {
    LightBeam lightbeam = light_beam_from_source(ray + ray.direction()*distance, light, distance_light - distance);

    if(inside_shape) {
      lightbeam.absorb(inside_shape->filter(), distance);
    }

    return lightbeam;
  }
}

const Shape* Scene::get_intersection_shape(const Ray &ray, const Shape* inside_shape, Plane &intersection_plane) const
{
  const Shape *intersection_shape = 0;
  double distance;
	
  for(int i=_shapes.size()-1; i>=0; i--) {
    Plane plane_i;
    if(_shapes[i]->intersect(ray, plane_i)) {
      double distance_i = (plane_i.origin() - ray.origin()).norm();
      if(is_greater(distance_i, 0) &&
         (!intersection_shape || is_greater(distance, distance_i))) {
        distance = distance_i;
	intersection_plane = plane_i;
	intersection_shape = _shapes[i];
      }
    }
    if(_shapes[i] == inside_shape)
      break;
  }
  return intersection_shape;
}

const Shape* Scene::get_inside_shape(const Ray &ray) const 
{
  for(int i=_shapes.size()-1; i>=0; i--) {
    if(_shapes[i]->inside( ray )) {
      return _shapes[i];
    }
  }
  return 0;
}

void Scene::clear_tracking() const
{
  _tracking_data.clear();
}

void Scene::start_tracking() const
{
  if(_tracking_active) {
    display_error("Scene: Tracking already running.");
  }
  _tracking_active = true;
}

void Scene::stop_tracking() const
{
  if(!_tracking_active) {
    display_error("Scene: Tracking not running.");
  }
  _tracking_active = false;
}

