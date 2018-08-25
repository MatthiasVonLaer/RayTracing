//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "shape.h"
#include "../utilities.h"

#include <fstream>
#include <string>

class Scene;

class Composition : public Shape
{
private:
  const Composition *_parent;
  Scene *_scene;

  double _scale;

  std::string _filename;
  std::string _path;
  std::ifstream _input;

public:
  Composition(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in) override;
  void setup(Scene *scene, const std::string &path);

  double init_dx() override {return _scale;}
  double init_dy() override {return _scale;}
  double init_dz() override {return _scale;}

  const Matrix& transformation_matrix() const {return Shape::transformation_matrix();}

  std::optional<Plane> intersect(const Ray &ray) const override;
  bool inside(const Ray &ray) const override;
  Color get_color(const Vector &Vector) const override;

  std::string name() const;
};
