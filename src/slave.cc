#include <iostream>
#include <string>
#include <unistd.h>

#include "slave.h"

using namespace std;

Slave::Slave() :
  _idle_sleep(true)
{
}

void Slave::loop()
{
  while(true) {

    int flag = 0;
    while(_idle_sleep && !flag) {
      MPI_Iprobe(0, MPI_ANY_TAG, mpi.comm(), &flag, 0);
      usleep(1000);
    }

    Order order;
    mpi.recv_order(order);
    
    if     (order == ORDER_QUIT)        break;
    else if(order == ORDER_PARSE)       parse();
    else if(order == ORDER_INIT)        init();
    else if(order == ORDER_CAMERA_DATA) camera_data();
    else if(order == ORDER_RAYTRACE)    raytrace();
    else if(order == ORDER_IDLE_SLEEP)  set_idle();
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
  _scene.init();
}

void Slave::camera_data()
{
  mpi.recv_int(_camera_resolution_x);
  mpi.recv_vector(_camera_position);
  mpi.recv_vector(_camera_film_top_left_direction);
  mpi.recv_vector(_camera_film_dx);
  mpi.recv_vector(_camera_film_dy);
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

void Slave::set_idle()
{
  int idle;
  mpi.recv_int(idle);
  _idle_sleep = idle;
}
