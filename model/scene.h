//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "light_beam.h"
#include "shapes/composition.h"
#include "shapes/light_source.h"
#include "shapes/shape.h"
#include "tracking_data.h"

#include <istream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Scene
{
private:
  std::map < std::string, std::shared_ptr<Shape> > _shapes_by_name;

  std::vector < std::shared_ptr<Shape> > _shapes;
  std::vector < std::shared_ptr<LightSource> > _lights;
  std::vector < std::shared_ptr<Composition> > _compositions;

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

public:
  void parse(const std::string &line, Composition *parent=nullptr);
private:
  template<class T> void get_shape_and_parse(const std::string &shape_class, std::istream &in, Composition *parent);
  template<class T> std::shared_ptr<T> create_shape(const std::string &name, Composition *parent);
  void parse_shape(const std::string &name, std::istream &in);
  void remove_shape(const std::string &name);

public:
  void init();
  LightBeam raytracer(const Ray &ray) const;

private:
  LightBeam raytracer(const Ray &ray, const Shape *inside_shape, double ratio, int depth) const;

  LightBeam reflection(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape,
                       double ratio, int depth) const;

  LightBeam refraction(const Ray &ray, const Plane &intersection_plane, const Shape *inside_shape,
                       const Shape*inside_shape_2, const Shape *interseciton_shape, double ratio, int depth) const;

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
