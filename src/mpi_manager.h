#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include <mpi.h>

#include "light_beam.h"
#include "vector.h"

class MPI_Manager;
extern MPI_Manager mpi;

enum Order {
  ORDER_QUIT,
  ORDER_PARSE,
  ORDER_INIT,
  ORDER_CAMERA_DATA,
  ORDER_RAYTRACE
};

class MPI_Manager
{
public:
  MPI_Manager();

  void init(int argc, char** argv);
  void finalize();
    
  unsigned int rank() const  { return _rank; }
  unsigned int size() const  { return _size; }
  MPI_Comm  get_comm() const { return _comm; }


  void recv_order (Order    &i, int source=0);
  void send_order (Order     i, int dest  =0);

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

 private:
  const int tag_data;
  const int tag_order;

  MPI_Comm _comm;
  int _size;
  int _rank;
  MPI_Status status;

  static bool singleton_exists;
};
