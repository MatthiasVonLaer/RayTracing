//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "shape.h"
#include "../color.h"
#include "../filter.h"
#include "../ray.h"
#include "../vector.h"

class Ball : public Shape
{
private:
  double _radius;
  QImage _image;

public:
  Ball(Composition *parent = nullptr);

  virtual void parse(const std::string &command, std::istream &in) override;

private:
  double init_dx() override {return _radius;}
  double init_dy() override {return _radius;}
  double init_dz() override {return _radius;}

public:
  std::optional<Plane> intersect(const Ray &ray) const override;
  bool inside(const Ray &ray) const override;
  Color get_color(const Vector &intersection_point) const override;

protected:
  void set_radius(double r) {_radius = r;}
};
