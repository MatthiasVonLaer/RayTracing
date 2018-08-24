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

#include "gui.h"
#include "mpi_manager.h"
#include "utilities.h"

#include <QApplication>


using namespace std;

Controller::Controller(QApplication &app) :
  _app(app),
  _scene(),
  _display(),
  _camera(_scene, _display),
  _ray_diagram(_scene, _camera)
{
}

void Controller::parse(istream &in)
{
  string command;
  string rest_of_line;

  while(in >> command && getline(in, rest_of_line)) {
    _display.progress("Loading", 0);

    command      =      command.substr(0,       command.find("//") );
    rest_of_line = rest_of_line.substr(0,  rest_of_line.find("//") );

    stringstream stream(rest_of_line);

    if(command.size() == 0) {
      continue;
    }
    else if(command == "launch_gui") {
      launch_gui();
    }
    else if(command == "take_picture") {
      take_picture();
    }
    else if(command == "quit") {
      break;
    }
    else if(command == "camera") {
      string command2;
      while(stream >> command2) {
        _camera.parse(command2, stream);
      }
    }
    else if(command == "display") {
      string command2;
      while(stream >> command2) {
        _display.parse(command2, stream);
      }
    }
    else if(command == "ray_diagram") {
      string command2;
      while(stream >> command2) {
        _ray_diagram.parse(command2, stream);
      }
    }
    else {
      _scene.parse(command + rest_of_line);

      for(int i=1; i<mpi().size(); i++) {
        mpi().send_order(MPI_Order::PARSE, i);
        mpi().send_string(command + rest_of_line, i);
      }
    }
  }

  //quit
  _display.summary();

  for(int i=1; i<mpi().size(); i++) {
    mpi().send_order(MPI_Order::QUIT, i);
  }
}

void Controller::initialize()
{
  _camera.initialize();

  if(_ray_diagram.enabled()) {
    _ray_diagram.init();
  }

  _scene.init();
  for(int i=1; i<mpi().size(); i++) {
    mpi().send_order(MPI_Order::INIT, i);
  }
}

void Controller::take_picture()
{
  initialize();

  _camera.take_picture();

  if(_ray_diagram.enabled()) {
    _ray_diagram.track();
    _ray_diagram.save();
  }

  _display.progress("Done", 1);
  cout << endl;
}

void Controller::launch_gui()
{
  _display.progress("Gui", 0);

  Gui gui(_scene, _camera);
  gui.showMaximized();
  _app.exec();
}