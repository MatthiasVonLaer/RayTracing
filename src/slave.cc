#include <iostream>
#include <string>

#include "slave.h"

using namespace std;

Slave::Slave()
{
}

void Slave::loop()
{
  while(true) {
    Order order;
    mpi.recv_order(order);
    
    if     (order == ORDER_QUIT)      break;
    else if(order == ORDER_PARSE)     parse();
    else if(order == ORDER_INIT)      init();
    else if(order == ORDER_RAYTRACE)  raytrace();
  }
}

void Slave::parse()
{
  string line;
  mpi.recv_string(line);

  _scene.parse(line);
}

void Slave::init()
{
  mpi.recv_int(_camera_resolution_x);
  mpi.recv_vector(_camera_position);
  mpi.recv_vector(_camera_film_top_left_direction);
  mpi.recv_vector(_camera_film_dx);
  mpi.recv_vector(_camera_film_dy);

  _scene.init();
}

void Slave::raytrace()
{
  int row;
  mpi.recv_int(row);

  vector<LightBeam> light_beams;
  light_beams.resize(_camera_resolution_x);
  for(int i=0; i<_camera_resolution_x; i++) {
    Vector v = _camera_film_top_left_direction + row*_camera_film_dy + i*_camera_film_dx;
    light_beams[i] = _scene.raytracer(Ray(_camera_position, v));
  }

  mpi.send_int(mpi.rank());
  mpi.send_int(row);
  for(int i=0; i<_camera_resolution_x; i++) {
    mpi.send_light_beam(light_beams[i]);
  }

}
