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

#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include "mpi_manager.h"

using namespace std;

bool MPI_Manager::singleton_exists = false;

MPI_Manager::MPI_Manager()
  : tag_data(0), tag_order(1)
{
  assert(!singleton_exists);
  singleton_exists = true;
}

void MPI_Manager::init(int argc, char** argv)
{
  MPI_Init(&argc,&argv);

  _comm = MPI_COMM_WORLD;
  
  MPI_Comm_rank(_comm,&_rank);
  MPI_Comm_size(_comm,&_size);
}

void MPI_Manager::finalize()
{
  MPI_Finalize();
}


void MPI_Manager::recv_order(Order &i,int source)
{
  assert(MPI_Recv(&i,1,MPI_INT,source,tag_order,_comm,&_status)==0);
}

void MPI_Manager::send_order(Order i,int dest)
{
  assert(MPI_Ssend(&i,1,MPI_INT,dest,tag_order,_comm)==0);
}

void MPI_Manager::recv_int(int &i,int source)
{
  assert(MPI_Recv(&i,1,MPI_INT,source,tag_data,_comm,&_status)==0);
}

void MPI_Manager::send_int(int i,int dest)
{
  assert(MPI_Ssend(&i,1,MPI_INT,dest,tag_data,_comm)==0);
}

void MPI_Manager::recv_double(double &i,int source)
{
  assert(MPI_Recv(&i,1,MPI_DOUBLE,source,tag_data,_comm,&_status)==0);
}

void MPI_Manager::send_double(double i,int dest)
{
  assert(MPI_Ssend(&i,1,MPI_DOUBLE,dest,tag_data,_comm)==0);
}

void MPI_Manager::send_string(const std::string &s,int dest)
{
  int size = s.size();
  send_int(size,dest);
  assert(MPI_Ssend((void *) s.c_str(),s.size(),MPI_CHAR,dest,tag_data,_comm)==0);
}

void MPI_Manager::recv_string(std::string &s,int source)
{
  int size;
  recv_int(size,source);
  s.resize(size);
  
  assert(MPI_Recv((void *) s.c_str(),s.size(),MPI_CHAR,source,tag_data,_comm,&_status)==0);  
}

void MPI_Manager::recv_vector(      Vector &v, int source)
{
  double x, y, z;
  recv_double(x, source);
  recv_double(y, source);
  recv_double(z, source);
  v = Vector(x, y, z);
}

void MPI_Manager::send_vector(const Vector &v, int dest)
{
  send_double(v.x(), dest);
  send_double(v.y(), dest);
  send_double(v.z(), dest);
}

void MPI_Manager::recv_light_beam(      LightBeam &lb, int source)
{
  double r, g, b, d;
  recv_double(r, source);
  recv_double(g, source);
  recv_double(b, source);
  recv_double(d, source);
  lb = LightBeam(r, g, b, d);
}

void MPI_Manager::send_light_beam(const LightBeam &lb, int dest)
{
  send_double(lb.red(), dest);
  send_double(lb.green(), dest);
  send_double(lb.blue(), dest);
  send_double(lb.depth(), dest);
}
