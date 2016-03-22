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

#pragma once

#include <istream>
#include <map>
#include <string>
#include <vector>

#include "composition.h"
#include "light_beam.h"
#include "light_source.h"
#include "shape.h"
#include "tracking_data.h"

class Scene
{
private:
  std::map < std::string, Shape* > _shapes_by_name;

  std::vector < Shape* > _shapes;
  std::vector < LightSource* > _lights;
  std::vector < Composition* > _compositions;

  LightBeam _sky;
  LightBeam _ambient_light;
  double _fog;

  double _recursion_break_ratio;
  int _recursion_break_depth;

  std::string _compositions_dir;

  mutable std::vector< TrackingData > _tracking_data;
  mutable bool _tracking_active;


public:
  Scene();
  ~Scene();

public:
  void parse(const std::string &line, Composition *parent=0);
private:
  template<class T> void get_or_create_shape_and_parse(const std::string &shape_class, std::istream &in, Composition *parent);
  void remove_shape(const std::string &name);

public:
  void init();
  LightBeam raytracer(const Ray &ray) const;

private:
  LightBeam raytracer(const Ray &ray, const Shape *inside_shape, double ratio, int depth) const;
  LightBeam reflection(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, double ratio, int depth) const;
  LightBeam refraction(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape, const Shape*inside_shape_2,
                       const Shape *interseciton_shape, double ratio, int depth) const;
  LightBeam scattered_light(const Plane &intersection_plane, const Color &color) const;
  LightBeam light_beam_from_source(const Ray &ray, const LightSource *light, double distance_light) const;
  const Shape* get_intersection_shape(const Ray &ray, const Shape *inside_shape, Plane &intersection_plane) const;
  const Shape* get_inside_shape(const Ray &ray) const;

public:
  void clear_tracking() const;
  void start_tracking() const;
  void stop_tracking() const;
  const std::vector<TrackingData>& tracking_data() const {return _tracking_data;}
};
