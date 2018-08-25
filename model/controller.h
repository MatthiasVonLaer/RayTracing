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
#include "ray_diagram.h"
#include "scene.h"

#include <istream>

class Controller
{
public:
  void parse(std::istream &in);
  Camera& camera() { return _camera; }
  Scene& scene() { return _scene; }

private:
  void initialize();
  void take_picture();
  void launch_gui();

  Scene _scene;
  Display _display;
  Camera _camera{_scene, _display};
  RayDiagram _ray_diagram{_scene, _camera};
};
