//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "light_beam.h"
#include "mpi_manager.h"
#include "vector.h"

#include <unistd.h>

using namespace std;

enum Tag { DATA, ORDER };

MPI_Manager& MPI_Manager::singleton()
{
  static MPI_Manager mpi_manager;
  return mpi_manager;
}

MPI_Manager::~MPI_Manager()
{
  if(_started)
    MPI_Finalize();
}

void MPI_Manager::init(int argc, char** argv)
{
  MPI_Init(&argc,&argv);

  _comm = MPI_COMM_WORLD;
  
  MPI_Comm_rank(_comm,&_rank);
  MPI_Comm_size(_comm,&_size);

  _started = true;
}

void MPI_Manager::iprobe(int idle_time)
{
  int flag = 0;
  while(!flag) {
    MPI_Iprobe(0, MPI_ANY_TAG, _comm, &flag, 0);
    usleep(idle_time);
  }
}

void MPI_Manager::recv_order(MPI_Order &i,int source)
{
  MPI_Recv(&i,1,MPI_INT,source,ORDER,_comm,&_status);
}

void MPI_Manager::send_order(MPI_Order i,int dest)
{
  MPI_Ssend(&i,1,MPI_INT,dest,ORDER,_comm);
}

void MPI_Manager::recv_int(int &i,int source)
{
  MPI_Recv(&i,1,MPI_INT,source,DATA,_comm,&_status);
}

void MPI_Manager::send_int(int i,int dest)
{
  MPI_Ssend(&i,1,MPI_INT,dest,DATA,_comm);
}

void MPI_Manager::recv_double(double &i,int source)
{
  MPI_Recv(&i,1,MPI_DOUBLE,source,DATA,_comm,&_status);
}

void MPI_Manager::send_double(double i,int dest)
{
  MPI_Ssend(&i,1,MPI_DOUBLE,dest,DATA,_comm);
}

void MPI_Manager::send_string(const std::string &s,int dest)
{
  int size = s.size();
  send_int(size,dest);
  MPI_Ssend((void *) s.c_str(),s.size(),MPI_CHAR,dest,DATA,_comm);
}

void MPI_Manager::recv_string(std::string &s,int source)
{
  int size;
  recv_int(size,source);
  s.resize(size);
  
  MPI_Recv((void *) s.c_str(),s.size(),MPI_CHAR,source,DATA,_comm,&_status);
}

void MPI_Manager::recv_vector(      Vector &v, int source)
{
  double x, y, z;
  recv_double(x, source);
  recv_double(y, source);
  recv_double(z, source);
  v = Vector(x, y, z);
}

void MPI_Manager::send_vector(const Vector &v, int dest)
{
  send_double(v.x(), dest);
  send_double(v.y(), dest);
  send_double(v.z(), dest);
}

void MPI_Manager::recv_light_beam(      LightBeam &lb, int source)
{
  double r, g, b, d;
  recv_double(r, source);
  recv_double(g, source);
  recv_double(b, source);
  recv_double(d, source);
  lb = LightBeam(r, g, b, d);
}

void MPI_Manager::send_light_beam(const LightBeam &lb, int dest)
{
  send_double(lb.red(), dest);
  send_double(lb.green(), dest);
  send_double(lb.blue(), dest);
  send_double(lb.depth(), dest);
}

MPI_Manager& mpi() {return MPI_Manager::singleton();}
