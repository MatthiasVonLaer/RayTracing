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

#include "mpi_manager.h"
#include "scene.h"
#include "vector.h"

class Slave
{
private:
  Scene _scene;

  int    _camera_resolution_x;
  Vector _camera_position;
  Vector _camera_film_top_left_direction;
  Vector _camera_film_dx;
  Vector _camera_film_dy;

  bool _idle_sleep;

public:
  Slave();
  void loop();

private:
  void parse();
  void init();
  void camera_data();
  void raytrace();
  void set_idle();
};
