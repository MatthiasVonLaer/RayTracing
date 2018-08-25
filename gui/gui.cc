//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "gui.h"

#include <QCursor>
#include <QKeyEvent>
#include <QPainter>

#include <iostream>

using namespace std;

Gui::Gui(const Scene &scene, Camera &camera) :
  _camera(camera),
  _scene(scene),
  _width(0),
  _height(0),
  _take_pic(false),
  _res_x(_camera.resolution_x()),
  _res_y(_camera.resolution_y()),
  _top_direction(_camera.top_direction()),
  _speed(.5),
  _timer(this)
{
  _scene.clear_tracking();

  double ratio = sqrt(300.*200. / (_res_x * _res_y));
  _res_live_x = ratio * _res_x;
  _res_live_y = ratio * _res_y;

  QPalette pal;
  pal.setColor(QPalette::Background, Qt::black);
  setPalette(pal);

  connect(&_timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
}

void Gui::paintEvent(QPaintEvent *event)
{
  if(!_width || !_height) {
    return;
  }

  QPainter painter(this);

  painter.drawImage(QRect(0, 0, _width, _height), _camera.film());

  const vector<TrackingData> &data = _scene.tracking_data();

  for(int i=0; i<data.size(); i++) {
    Ray ray          = data[i].ray;
    double distance0 = 0;
    double distance1 = data[i].distance;
    double ratio     = data[i].ratio;

    bool in_front_0 = _camera.viewing_direction()
                    * (ray.origin() + distance0 * ray.direction() - _camera.position()) > 0;
    bool in_front_1 = _camera.viewing_direction()
                    * (ray.origin() + distance1 * ray.direction() - _camera.position()) > 0;

    double dist_film;
    ray.intersect( Plane(_camera.position(), _camera.viewing_direction()), dist_film );

    if(!in_front_0 && !in_front_1) {
      continue;
    }
    else if(!in_front_0) {
      distance0 = dist_film + 1e-3;
    }
    else if(!in_front_1) {
      distance1 = dist_film - 1e-3;
    }

    Vector v1 = _camera.film_coordinates(ray.origin() + distance0 * ray.direction());
    Vector v2 = _camera.film_coordinates(ray.origin() + distance1 * ray.direction());

    v1 *= static_cast<double>(_width) / _camera.film().width();
    v2 *= static_cast<double>(_width) / _camera.film().width();

    painter.setPen(QPen(QColor(255*ratio, 0, 0), 2));
    painter.drawLine(v1.x(), v1.y(), v2.x(), v2.y());
    painter.setPen(QPen(QColor(255*ratio, 0, 0), 5));
    painter.drawPoint(v2.x(), v2.y());
  }
}

void Gui::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Escape) {
    close();
  }
  else if(event->key() == Qt::Key_Space) {

    if(!_timer.isActive()) {
      _camera.set_resolution(_res_live_x, _res_live_y);

      _timer.start(50);

      QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));
      setMouseTracking(true);
    }
    else {
      _camera.set_resolution(_res_x, _res_y);

      _timer.stop();

      setMouseTracking(false);
    }

    _camera.initialize();
    _camera.take_picture();
    
    update();
  }
  else if(event->key() == 's') {
    if(!_timer.isActive()) {
      _camera.save();
    }
  }

  if(_timer.isActive()) {
    Vector right = _camera.viewing_direction() ^ _camera.top_direction();
    if(event->key() == Qt::Key_Left) {
      _camera.set_position( _camera.position() - _speed * right );
      _take_pic = true;
    }
    else if(event->key() == Qt::Key_Right) {
      _camera.set_position( _camera.position() + _speed * right );
      _take_pic = true;
    }
    else if(event->key() == Qt::Key_Up) {
      _camera.set_position( _camera.position() + _speed * _camera.viewing_direction() );
      _take_pic = true;
    }
    else if(event->key() == Qt::Key_Down) {
      _camera.set_position( _camera.position() - _speed * _camera.viewing_direction() );
      _take_pic = true;
    }
    else if(event->key() == Qt::Key_PageUp) {
      _camera.set_position( _camera.position() + _speed * _camera.top_direction() );
      _take_pic = true;
    }
    else if(event->key() == Qt::Key_PageDown) {
      _camera.set_position( _camera.position() - _speed * _camera.top_direction() );
      _take_pic = true;
    }
    else if(event->key() == '*') {
      _speed *= 1.5;
    }
    else if(event->key() == '/') {
      _speed /= 1.5;
    }
    else if(event->key() == Qt::Key_Plus) {
      _camera.set_exposure( _camera.exposure() + .333 );
      _take_pic = true;
    }
    else if(event->key() == Qt::Key_Minus) {
      _camera.set_exposure( _camera.exposure() - .333 );
      _take_pic = true;
    }
  }
}

void Gui::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton) {
    int x = (event->x()-1) * _camera.resolution_x() / _width;
    int y = (event->y()-1) * _camera.resolution_y() / _height;

    _scene.start_tracking();
    Vector v = _camera.film_top_left_direction() + y*_camera.film_dy() + x*_camera.film_dx();
    _scene.raytracer(Ray(_camera.position(), v));
    _scene.stop_tracking();

  }
  else if(event->button() == Qt::RightButton) {
    _scene.clear_tracking();
  }

  update();
}

void Gui::resizeEvent(QResizeEvent *event)
{
  const QImage &image = _camera.film();
  if(static_cast<double>(image.width()) / width() > static_cast<double>(image.height()) / height()) {
    _width = width();
    _height = width() * image.height() / image.width();
  }
  else {
    _height = height();
    _width = height() * image.width() / image.height();
  }
  update();
}

void Gui::mouseMoveEvent(QMouseEvent *event)
{
  if(_timer.isActive()) {
    int x = event->x() - width()/2;   
    int y = event->y() - height()/2;   

    Vector direction = _camera.viewing_direction();
    direction -= y/4000. * _camera.top_direction();
    direction += x/2000. * (_camera.viewing_direction() ^ _camera.top_direction());
    _camera.set_viewing_direction(direction);
    _camera.set_top_direction(_top_direction);

    _take_pic = true;

    QCursor::setPos(mapToGlobal(QPoint(width()/2, height()/2)));
  }
}

void Gui::wheelEvent(QWheelEvent *event)
{
  if(_timer.isActive()) {
    if(event->delta() > 0) {
      _camera.set_focal_length( _camera.focal_length() * event->delta()/100. );
      _take_pic = true;
    }
    else if(event->delta() < 0) {
      _camera.set_focal_length( _camera.focal_length() / (-event->delta()/100.) );
      _take_pic = true;
    }
  }
}

void Gui::timerEvent()
{
  if(_take_pic) {
    _camera.initialize();
    _camera.take_picture();
    _take_pic = false;
  }
  update();
}
