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

#include "shape.h"
#include "utilities.h"

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

  std::optional<Plane> intersect(const Ray &ray) const override {display_error("Composition should be invisible");}
  bool inside(const Ray &ray) const override                  {display_error("Composition should be invisible");}
  Color get_color(const Vector &Vector) const override        {display_error("Composition should be invisible");}

  std::string name() const;
};
