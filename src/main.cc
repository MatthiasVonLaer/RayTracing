#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "camera.h"
#include "mpi_manager.h"
#include "slave.h"

#include "math_expression.h"

using namespace std;

MPI_Manager mpi;

int main(int argc, char *argv[])
{
  mpi.init(argc,argv);

  if(mpi.rank() == 0) {

    if(argc != 2) {
      display_error("Usage: " + string(argv[0]) + " [input_filename]");
    }

    ifstream in(argv[1]);  
    if(!in.is_open()) {
      display_error("Can't open file " + string(argv[1]));
    }

    Camera camera;
    camera.parse(in);

  }
  else {
    Slave slave;
    slave.loop();
  }

  mpi.finalize();
  return 0;
}
