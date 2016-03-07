#include <iostream>
#include <sstream>
#include <set>
#include <typeinfo>
#include <assert.h>

#include "ball.h"
#include "cube.h"
#include "function_plot.h"
#include "plane_shape.h"
#include "scene.h"

using namespace std;

Scene::Scene() :
  _ray_diagram(0)
{
  _fog = 0;
  _recursion_break_ratio = 0.1;
}

Scene::~Scene()
{
  for(int i=0; i<_shapes.size(); i++) {
    delete _shapes[i];
  }
}

LightBeam Scene::raytracer(const Ray &ray) const
{
  return raytracer(ray, get_inside_shape(ray), 1, 0);
}

void Scene::parse(const string &line)
{
  stringstream stream(line);
  string command;

  stream >> command;

  if(command == "ambient_light") {
    stream >> _ambient_light;
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
    remove_shape(name1 + name2);
  }
  else if(command == "sky") {
    stream >> _sky;
  }
  //shapes
  else if(command == "ball") {
    get_or_create_shape_and_parse<Ball>(command, stream);
  }
  else if(command == "cube") {
    get_or_create_shape_and_parse<Cube>(command, stream);
  }
  else if(command == "function_plot") {
    get_or_create_shape_and_parse<FunctionPlot>(command, stream);
  }
  else if(command == "light") {
    get_or_create_shape_and_parse<LightSource>(command, stream);
  }
  else if(command == "plane") {
    get_or_create_shape_and_parse<PlaneShape>(command, stream);
  }
  else {
    parser_error_unknown_command(command);
  }

}

void Scene::init()
{
  for(int i=_shapes.size()-1; i>=0; i--) {
    if(!_shapes[i]->invisible()) {
      _shapes[i]->init_unit_vectors();
      _shapes[i]->init();
      _shapes[i]->init_transformation_matrix();
    }
    if(_shapes[i]->invisible()) {
      _shapes.erase(_shapes.begin() + i);
    }
  }
}

void Scene::set_ray_diagram(RayDiagram *rd)
{
  _ray_diagram = rd;
}

template<class T> void Scene::get_or_create_shape_and_parse(const std::string &shape_class, std::istream &in)
{
  string name;
  in >> name;
  name = shape_class + name;

  Shape *shape;

  if(!_shapes_by_name.count(name)) {
    shape = new T;
    _shapes_by_name[name] = shape;
    _shapes.push_back(shape);
    if(typeid(T) == typeid(LightSource)) {
      _lights.push_back( dynamic_cast<LightSource*>(shape) );
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

  for(int i=0; i<_shapes.size(); i++) {
    if(_shapes[i] == _shapes_by_name[name]) {
      delete _shapes[i];
      _shapes.erase(_shapes.begin() + i);
      _shapes_by_name.erase(name);
      return;
    }
  }

  for(int i=0; i<_lights.size(); i++) {
    if(_lights[i] == _shapes_by_name[name]) {
      delete _lights[i];
      _lights.erase(_lights.begin() + i);
      _shapes_by_name.erase(name);
      return;
    }
  }

  display_error("shape to remove " + name + " not found.");
}

LightBeam Scene::raytracer(const Ray &ray, const Shape *inside_shape, double ratio, int depth) const
{
  if(ratio < _recursion_break_ratio || depth >= _recursion_break_depth)
    return LightBeam();

  //Get intersection point
  Plane intersection_plane;
  const Shape *intersection_shape = get_intersection_shape(ray, inside_shape, intersection_plane);
  double distance = (ray.origin() - intersection_plane.origin()).norm();
  
  //Get Lightbeam
  LightBeam lightbeam;
  if(intersection_shape) {
    //MIRROR
    if(is_equal(intersection_shape->silvered(), 1))
      lightbeam = reflection(ray, intersection_plane, inside_shape, ratio, depth+1);

    //TRANSPARENT
    else if(intersection_shape->color_type() == TRANSPARENT) {
      lightbeam = refraction(ray, intersection_plane, inside_shape, intersection_shape, ratio, depth+1);
    }

    //OPAQUE
    else {
      lightbeam =
        reflection(ray, intersection_plane, inside_shape, ratio * intersection_shape->silvered(), depth+1) * intersection_shape->silvered() +
        scattered_light(intersection_plane, intersection_shape->get_color(intersection_plane.origin())) * (1 - intersection_shape->silvered());
    }

    //FILTER
    if(inside_shape) {
      Filter filter = inside_shape->filter() ^ distance;
      lightbeam = lightbeam * filter;
    }
    //FOG
    else {
      double fog = exp(_fog * distance);
      lightbeam = lightbeam*fog + _ambient_light*(1-fog);
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

  if(_ray_diagram) {
    _ray_diagram->add(ray, distance, ratio);
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

LightBeam Scene::refraction(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, const Shape *intersection_shape, double ratio, int depth) const
{
  if(intersection_shape->shape_type() == SURFACE)
    return raytracer(Ray(intersection_plane.origin(), ray.direction()), inside_shape, ratio, depth) * intersection_shape->filter();

  const Shape *inside_shape_2 = get_inside_shape(Ray(intersection_plane.origin(), ray.direction() ));

	
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

    return reflection(ray, intersection_plane, inside_shape, ratio*reflection_coefficient, depth) * reflection_coefficient
      + raytracer(refraction_ray, inside_shape_2, ratio*(1-reflection_coefficient), depth) * (1-reflection_coefficient);
  }
  else
    return reflection(ray, intersection_plane, inside_shape, ratio, depth);
}

LightBeam Scene::scattered_light(const Plane &intersection_plane, const Color &color) const
{
  LightBeam light_beam = _ambient_light;

  for(int i=0; i<_lights.size(); i++) {
    Ray light_ray(intersection_plane.origin(),
                  _lights[i]->position() - intersection_plane.origin());

    double distance = (_lights[i]->position() - intersection_plane.origin()).norm();

    double fog = exp(-_fog * distance);

    light_beam += light_beam_from_source(light_ray, _lights[i], distance)
                  * (fabs(light_ray.direction() * intersection_plane.normal()) / (4*PI*distance*distance)
                  * fog);
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
  else if(intersection_shape->color_type() != TRANSPARENT) {
    return LightBeam(); 
  }
  else {
    Filter filter;
    if(inside_shape) {
      filter = inside_shape->filter() ^ distance;
    }
    if(intersection_shape->shape_type() == SURFACE) {
      filter *= intersection_shape->filter();
    }

    return light_beam_from_source(ray + ray.direction()*distance, light, distance_light - distance) * filter;
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
