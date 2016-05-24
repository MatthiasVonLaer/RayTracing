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

#include "slave.h"

#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

Slave::Slave() :
  _idle_sleep(true)
{
}

void Slave::loop()
{
  while(true) {

    if(_idle_sleep) {
      mpi().iprobe(1000);
    }

    MPI_Order order;
    mpi().recv_order(order);
    
    if     (order == MPI_Order::QUIT)        break;
    else if(order == MPI_Order::PARSE)       parse();
    else if(order == MPI_Order::INIT)        init();
    else if(order == MPI_Order::CAMERA_DATA) camera_data();
    else if(order == MPI_Order::RAYTRACE)    raytrace();
    else if(order == MPI_Order::IDLE_SLEEP)  set_idle();
  }
}

void Slave::parse()
{
  string line;
  mpi().recv_string(line);

  _scene.parse(line);
}

void Slave::init()
{
  _scene.init();
}

void Slave::camera_data()
{
  mpi().recv_int(_camera_resolution_x);
  mpi().recv_vector(_camera_position);
  mpi().recv_vector(_camera_film_top_left_direction);
  mpi().recv_vector(_camera_film_dx);
  mpi().recv_vector(_camera_film_dy);
}

void Slave::raytrace()
{
  int row;
  mpi().recv_int(row);

  vector<LightBeam> light_beams;
  light_beams.resize(_camera_resolution_x);
  for(int i=0; i<_camera_resolution_x; i++) {
    Vector v = _camera_film_top_left_direction + row*_camera_film_dy + i*_camera_film_dx;
    light_beams[i] = _scene.raytracer(Ray(_camera_position, v));
  }

  mpi().send_int(mpi().rank());
  mpi().send_int(row);
  for(const auto& beam : light_beams) {
    mpi().send_light_beam(beam);
  }

}

void Slave::set_idle()
{
  int idle;
  mpi().recv_int(idle);
  _idle_sleep = idle;
}
