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
#include "scene.h"

#include <QTimer>
#include <QWidget>

class Gui : public QWidget
{
  Q_OBJECT
private:
  const Scene &_scene;
  Camera &_camera;
  int _width;
  int _height;

  QTimer _timer;

  bool _take_pic;
  double _speed;
  int _res_live_x;
  int _res_live_y;
  int _res_x;
  int _res_y;
  Vector _top_direction;

public:
  Gui(const Scene &scene, Camera &camera);

public slots:
  void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void timerEvent();
};
