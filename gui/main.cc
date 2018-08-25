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

#include "gui.h"

#include <model/controller.h>
#include <model/master.h>
#include <model/mpi_manager.h>
#include <model/slave.h>

#include <QApplication>

using namespace std;

int run(int argc, char** argv)
{
  mpi().init(argc,argv);

  if(mpi().rank() == 0)
  {
    Master master;

    if(argc != 2)
    {
      throw std::invalid_argument("Usage: raytracing_gui scene_file.");
    }

    ifstream input(argv[1]);
    if(!input.is_open())
    {
      throw std::runtime_error("Can't open file " + string(argv[1]));
    }

    Controller controller;
    controller.parse(input);
    QApplication app(argc, argv);
    Gui gui(controller.scene(), controller.camera());
    gui.showMaximized();
    app.exec();
  }
  else
  {
    Slave slave;
    slave.loop();
  }

  return EXIT_SUCCESS;
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
    return EXIT_SUCCESS;
  }
}
