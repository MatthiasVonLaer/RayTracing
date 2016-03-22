#pragma once

#include <istream>

#include <QApplication>

#include "camera.h"
#include "ray_diagram.h"
#include "scene.h"

class Controller
{
private:
  Camera _camera;
  Scene _scene;
  Display _display;
  RayDiagram _ray_diagram;
  QApplication &_app;

public:
  Controller(QApplication &app);
  void parse(std::istream &in);
private:
  void initialize();
  void take_picture();
  void launch_gui();
};
