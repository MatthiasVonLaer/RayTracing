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

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>

#include <QApplication>

#include "controller.h"
#include "mpi_manager.h"
#include "slave.h"

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
