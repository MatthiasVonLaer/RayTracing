//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "plane.h"
#include "shape.h"
#include "vector.h"

#include <vector>

class Cylinder : public Shape
{
private:
  Plane _plane[2];
  double _radius;

public:
  Cylinder(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in) override;

private:
  double init_dx() override {return _radius;}
  double init_dy() override {return _radius;}
  void init_derived_class() override;
	
public:
  std::optional<Plane> intersect(const Ray &ray) const override;
private:
  bool intersect_plane(const Ray &ray, Plane &intersection_plane) const;
  bool intersect_circle(const Ray &ray, Plane &intersection_plane) const;
public:
  bool inside(const Ray &ray) const override;
  Color get_color(const Vector &intersection_point) const override;
};
