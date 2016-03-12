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
  assert(MPI_Recv(&i,1,MPI_INT,source,tag_order,get_comm(),&status)==0);
}

void MPI_Manager::send_order(Order i,int dest)
{
  assert(MPI_Ssend(&i,1,MPI_INT,dest,tag_order,get_comm())==0);
}

void MPI_Manager::recv_int(int &i,int source)
{
  assert(MPI_Recv(&i,1,MPI_INT,source,tag_data,get_comm(),&status)==0);
}

void MPI_Manager::send_int(int i,int dest)
{
  assert(MPI_Ssend(&i,1,MPI_INT,dest,tag_data,get_comm())==0);
}

void MPI_Manager::recv_double(double &i,int source)
{
  assert(MPI_Recv(&i,1,MPI_DOUBLE,source,tag_data,get_comm(),&status)==0);
}

void MPI_Manager::send_double(double i,int dest)
{
  assert(MPI_Ssend(&i,1,MPI_DOUBLE,dest,tag_data,get_comm())==0);
}

void MPI_Manager::send_string(const std::string &s,int dest)
{
  int size = s.size();
  send_int(size,dest);
  assert(MPI_Ssend((void *) s.c_str(),s.size(),MPI_CHAR,dest,tag_data,get_comm())==0);
}

void MPI_Manager::recv_string(std::string &s,int source)
{
  int size;
  recv_int(size,source);
  s.resize(size);
  
  assert(MPI_Recv((void *) s.c_str(),s.size(),MPI_CHAR,source,tag_data,get_comm(),&status)==0);  
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
  double r, g, b;
  recv_double(r, source);
  recv_double(g, source);
  recv_double(b, source);
  lb = LightBeam(r, g, b);
}

void MPI_Manager::send_light_beam(const LightBeam &lb, int dest)
{
  send_double(lb.red(), dest);
  send_double(lb.green(), dest);
  send_double(lb.blue(), dest);
}
