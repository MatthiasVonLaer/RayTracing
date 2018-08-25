//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <assert.h>

#include <string>
#include <vector>

class MathOperation
{
private:
  std::vector<const double*> _args;
  double _result;
  double (*_func1)(double);
  double (*_func2)(double, double);

public:
  MathOperation(const std::string &pattern);

  void set_arg(int i, const double* d)  {assert(i<_args.size()); _args[i] = d;}

  void calculate();

  int number_args() const               {return _args.size();}
  const double* result() const          {return &_result;}
};

