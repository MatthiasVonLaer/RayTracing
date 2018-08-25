#include "mpi_manager.h"

#include <thread>

class Master
{
public:
  ~Master()
  {
    for(int i=1; i<mpi().size(); i++) {
      mpi().send_order(MPI_Order::QUIT, i);
    }
  }
};
