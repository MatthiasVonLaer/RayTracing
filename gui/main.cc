//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
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
