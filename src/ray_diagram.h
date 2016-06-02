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

#include "camera.h"
#include "matrix.h"
#include "ray.h"
#include "scene.h"
#include "shape.h"

#include <QImage>
#include <QPainter>

#include <string>

class RayDiagram
{
private:
  const Scene &_scene;
  const Camera &_camera;

  QImage _image;
  QPainter _painter;

  Matrix _transformation_matrix;

  double _range;
  int _width;
  int _height;
  int _y0;
  int _number_rays;

  std::string _output_file;

  bool _enabled;

public:
  RayDiagram(const Scene &scene, const Camera &camera);

  void parse(const std::string &command, std::istream &stream);
  void init();

public:
  void track();
private:
  void paint_ray(const Ray &ray, double distance, double ratio);
  void paint_shape(const Ray &ray, const Shape *shape, double distance);

public:
  void save() const;

  bool enabled() const  {return _enabled;}
};
