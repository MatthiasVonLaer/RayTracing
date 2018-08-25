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

#include <memory>
#include <vector>

class Cube : public Shape
{
private:
  Plane _plane[6];
  std::shared_ptr<QImage> _image[6];

public:
  Cube(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in) override;

private:
  void init_derived_class() override;
	
public:
  std::optional<Plane> intersect(const Ray &ray) const override;
  bool inside(const Ray &ray) const override;
  Color get_color(const Vector &intersection_point) const override;
};
