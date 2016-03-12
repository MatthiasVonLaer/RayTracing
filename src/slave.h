#pragma once

#include "mpi_manager.h"
#include "scene.h"
#include "vector.h"

class Slave
{
public:
  Slave();
  void loop();
private:
  void parse();
  void init();
  void camera_data();
  void raytrace();

private:
  Scene _scene;

  int    _camera_resolution_x;
  Vector _camera_position;
  Vector _camera_film_top_left_direction;
  Vector _camera_film_dx;
  Vector _camera_film_dy;
};
