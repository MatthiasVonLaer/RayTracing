//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "utilities.h"

#include "vector.h"

#include <math.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>
#include <QString>

using namespace std;

bool is_equal(double a, double b, double epsilon)
{
  return fabs(a - b) <= epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_greater(double a, double b, double epsilon)
{
  return a - b >= epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_greater_or_equal(double a, double b, double epsilon)
{
  return a - b >= -epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_smaller(double a, double b, double epsilon)
{
  return b - a >= epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_smaller_or_equal(double a, double b, double epsilon)
{
  return b - a >= -epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

double f_rand(double f_min, double f_max)
{
  double f = static_cast<double>(rand()) / RAND_MAX;
  return f_min + f * (f_max - f_min);
}

long get_time()
{
  timeval val;
  gettimeofday(&val, 0);
  return 1000000 * val.tv_sec + val.tv_usec;
}

bool file_exists(const string &path)
{
  ifstream in(path.c_str());
  return in.is_open();
}

void display_warning(const std::string &text)
{
  cerr << endl << "\033[0;33m" << text << "\033[0m" << endl;
}

void display_error(const std::string &text)
{
  cerr << endl << "\033[0;31m" << text << "\033[0m" << endl;
}

void parser_error_unknown_command(const std::string &command)
{
  throw std::runtime_error("Parser Error: Command " + command + " not known.");
}

void parser_assert_command(const std::string &command, const std::string &expected_command)
{
  if(command != expected_command) {
    throw std::runtime_error("Parser Error: Got keyword " + command + " instead of the expected keyword " + expected_command + ".");
  }
}
