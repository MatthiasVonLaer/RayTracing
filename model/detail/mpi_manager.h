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

#include <mpi.h>

#include <string>

class LightBeam;
class Vector;

enum class MPI_Order {
  QUIT,
  PARSE,
  INIT,
  CAMERA_DATA,
  RAYTRACE,
  IDLE_SLEEP
};

class MPI_Manager
{
private:
  MPI_Manager() = default;

public:
  static MPI_Manager& singleton();
  ~MPI_Manager();

  void init(int argc, char** argv);
  void iprobe(int idle_time);  

  void recv_order (MPI_Order &i, int source=0);
  void send_order (MPI_Order  i, int dest  =0);

  void recv_int   (int      &i, int source=0);
  void send_int   (int       i, int dest  =0);

  void recv_double(double   &i, int source=0);
  void send_double(double    i, int dest  =0);

  void recv_string(      std::string &s, int source=0);
  void send_string(const std::string &s, int dest  =0);

  void recv_vector(      Vector &v, int source=0);
  void send_vector(const Vector &v, int dest  =0);

  void recv_light_beam(      LightBeam &lb, int source=0);
  void send_light_beam(const LightBeam &lb, int dest  =0);


  int rank() const      { return _rank; }
  int size() const      { return _size; }

private:
  MPI_Comm _comm;
  int _size;
  int _rank;
  MPI_Status _status;
  bool _started = false;
};

MPI_Manager& mpi();
