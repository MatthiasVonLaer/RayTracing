#include <iostream>

#include "controller.h"
#include "mpi_manager.h"
#include "utilities.h"

using namespace std;

Controller::Controller() :
  _scene(0),
  _ray_diagram(),
  _display(),
  _camera(_display)
{
  if(mpi.size() == 1) {
    _scene = new Scene(&_ray_diagram);
  }
}

Controller::~Controller()
{
  delete _scene;
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
      if(mpi.size() > 1) {
        for(int i=1; i<mpi.size(); i++) {
          mpi.send_order(ORDER_PARSE, i);
          mpi.send_string(command + rest_of_line, i);
        }
      }
      else {
        _scene->parse(command + rest_of_line);
      }
    }
  }

  //quit
  _display.summary();

  for(int i=1; i<mpi.size(); i++) {
    mpi.send_order(ORDER_QUIT, i);
  }
}

void Controller::initialize()
{
  _camera.initialize();

  if(_ray_diagram.enabled()) {
    _ray_diagram.init(_camera);
  }

  if(mpi.size() == 1) {
    _scene->init();
  }
  else {
    for(int i=1; i<mpi.size(); i++) {
      mpi.send_order(ORDER_INIT, i);
    }
  }
}

void Controller::take_picture()
{
  initialize();

  _camera.take_picture(_scene);

  if(_ray_diagram.enabled()) {
    _ray_diagram.save();
  }

  _display.progress("Done", 1);
  cout << endl;
}
