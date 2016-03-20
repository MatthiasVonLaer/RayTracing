#pragma once

#include <QTimer>
#include <QWidget>

#include "camera.h"
#include "matrix.h"
#include "scene.h"

class Gui : public QWidget
{
  Q_OBJECT
private:
  const Scene &_scene;
  Camera &_camera;
  int _width;
  int _height;

  QTimer *_timer;

  bool _take_pic;
  double _speed;
  int _res_live_x;
  int _res_live_y;
  int _res_x;
  int _res_y;
  Vector _top_direction;

public:
  Gui(const Scene &scene, Camera &camera);
  ~Gui();

public slots:
  void paintEvent(QPaintEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);
  void timerEvent();
};
