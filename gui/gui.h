//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <model/camera.h>
#include <model/matrix.h>
#include <model/scene.h>

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
