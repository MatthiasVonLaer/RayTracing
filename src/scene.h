#pragma once

#include <istream>
#include <map>
#include <string>
#include <vector>

#include "light_beam.h"
#include "light_source.h"
#include "ray_diagram.h"
#include "shape.h"

class Scene
{
public:
  Scene();
  ~Scene();
  LightBeam raytracer(const Ray &ray) const;
  void parse(const std::string &line);
  void init();
  void set_ray_diagram(RayDiagram *rd);
private:
  template<class T> void get_or_create_shape_and_parse(const std::string &shape_class, std::istream &in);
  void remove_shape(const std::string &name);

  LightBeam raytracer(const Ray &ray, const Shape *inside_shape, double ratio, int depth) const;
  LightBeam reflection(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, double ratio, int depth) const;
  LightBeam refraction(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, const Shape *interseciton_shape, double ratio, int depth) const;
  LightBeam scattered_light(const Plane &intersection_plane, const Color &color) const;
  LightBeam light_beam_from_source(const Ray &ray, const LightSource *light, double distance_light) const;
  const Shape* get_intersection_shape(const Ray &ray, const Shape *inside_shape, Plane &intersection_plane) const;
  const Shape* get_inside_shape(const Ray &ray) const;

private:
  LightBeam _sky;
  LightBeam _ambient_light;
  double _fog;

  double _recursion_break_ratio;
  int _recursion_break_depth;

  std::vector < Shape* > _shapes;
  std::vector < LightSource* > _lights;

  std::map < std::string, Shape* > _shapes_by_name;

  RayDiagram *_ray_diagram;
};
