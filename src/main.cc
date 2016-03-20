#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include <QApplication>

#include "controller.h"
#include "mpi_manager.h"
#include "slave.h"

#include "lens.h"

using namespace std;

MPI_Manager mpi;

int main(int argc, char *argv[])
{
  mpi.init(argc,argv);

  if(mpi.rank() == 0) {

    QApplication app(argc, argv);
    Controller controller(app);

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
