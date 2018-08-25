//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "controller.h"

#include "mpi_manager.h"
#include "utilities.h"

#include <QApplication>


using namespace std;

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
      _scene.parse(command + rest_of_line);

      for(int i=1; i<mpi().size(); i++) {
        mpi().send_order(MPI_Order::PARSE, i);
        mpi().send_string(command + rest_of_line, i);
      }
    }
  }

  //quit
  _display.summary();
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
