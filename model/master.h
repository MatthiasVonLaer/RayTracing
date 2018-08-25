//  A raytracer program for rendering photorealistic images
//  Copyright 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "mpi_manager.h"

#include <thread>

class Master
{
public:
  ~Master()
  {
    for(int i=1; i<mpi().size(); i++) {
      mpi().send_order(MPI_Order::QUIT, i);
    }
  }
};
