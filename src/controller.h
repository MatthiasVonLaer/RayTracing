#pragma once

#include <istream>

#include "camera.h"
#include "ray_diagram.h"
#include "scene.h"

class Controller
{
public:
  Controller();
  ~Controller();
  void parse(std::istream &in);
  void initialize();
  void take_picture();

private:
  Camera _camera;
  RayDiagram _ray_diagram;
  Display _display;
  Scene *_scene;
};
