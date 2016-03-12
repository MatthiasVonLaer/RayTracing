#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include "controller.h"
#include "mpi_manager.h"
#include "slave.h"

#include "lens.h"

using namespace std;

MPI_Manager mpi;

int main(int argc, char *argv[])
{
  if(argc == 1) {
    Lens lens;
    lens.set_blades(4);
    lens.set_focal_length(.005);
    lens.set_aperture(22);
    double s = .00;
    s /= sqrt(2);
    cout << sqrt(norm(lens.diffraction_pattern(s, s))) << endl;
    return 0;
  }

  mpi.init(argc,argv);

  if(mpi.rank() == 0) {

    Controller controller;

    if(argc == 1) {
      controller.parse(cin);
    }
    else if(argc == 2) {
      ifstream input(argv[1]);
      if(input.is_open()) {
        controller.parse(input);
      }
      else {
        display_error("Can't open file " + string(argv[1]));
      }
    }
    else {
      display_error("Usage: Too many arguments.");
    }

  }
  else {
    Slave slave;
    slave.loop();
  }

  mpi.finalize();
  return 0;
}
