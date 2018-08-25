//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <model/controller.h>
#include <model/master.h>
#include <model/mpi_manager.h>
#include <model/slave.h>
#include <model/utilities.h>

using namespace std;

int run(int argc, char** argv)
{
  mpi().init(argc,argv);

  if(mpi().rank() == 0)
  {
    Master master;
    Controller controller;

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
