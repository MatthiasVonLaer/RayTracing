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

#include "controller.h"
#include "mpi_manager.h"
#include "slave.h"
#include "utilities.h"

#include <QApplication>

using namespace std;

int run(int argc, char** argv)
{
  mpi().init(argc,argv);

  if(mpi().rank() == 0) {

    QApplication app(argc, argv);
    Controller controller(app);

    //Piped
    if(argc == 1) {
      controller.parse(cin);
    }
    //Input file
    else if(argc == 2) {
      ifstream input(argv[1]);
      if(input.is_open()) {
        controller.parse(input);
      }
      else {
        throw std::runtime_error("Can't open file " + string(argv[1]));
      }
    }
    else {
      throw std::invalid_argument("Usage: Too many arguments.");
    }

  }
  else {
    Slave slave;
    slave.loop();
  }

  mpi().finalize();
  return 0;
}

int main(int argc, char** argv)
{
  try
  {
    return run(argc, argv);
  }
  catch (const std::exception& e)
  {
    display_error(e.what());
  }
}
