//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <math.h>

#include "root_finder.h"
#include "utilities.h"

using namespace std;

RootFinder::RootFinder() :
  _tolerance(1e-6),
  _min_step(0.01),
  _newton_steps(20),
  _nested_intervals_steps(30)
{
}

bool RootFinder::find(Function f, Function df, double min, double max, double &result) const
{
  double t0 = min;
  double t1 = min;
  double f1 = f(t1);

  double sign_start = f1/fabs(f1);

  while(is_smaller(t1, max)) {
    t0 = t1;
    t1 += _min_step * (max - min);

    if(t1 > max) {
      t1 = max;
    }
    f1 = f(t1);


    if(sign_start*f1 < _tolerance) {
      if(newton(f, df, t0, t1, _newton_steps, result)) {
        return true;
      }
      if(nested_intervals(f, t0, t1, _nested_intervals_steps, result)) {
        return true;
      }

      display_warning("Rootfinder: unable to locate root in desired accuracy");
      return true;
    }
  }
  return false;
}

bool RootFinder::nested_intervals(Function f, double t0, double t1, int steps, double &result) const
{
  double f0 = f(t0);
  double sign_start = f0/fabs(f0);
  
  if(result < t0 || result > t1) {
    result = (t0+t1)/2;
  }

  for(int i=0; i<steps; i++) {
    double ft = f(result);

    if(fabs(ft) < _tolerance) {
      return true;
    }

    if(sign_start*ft > 0) {
      t0 = result;
    }
    else {
      t1 = result;
    }

    result = (t0+t1)/2;
  }
  return false;
}

bool RootFinder::newton(Function f, Function df, double min, double max, int steps, double &result) const
{
  double alpha=1;
  double t = min;
  double f1, f0;

  f0 = f(t);
  
  for(int i=0; i<steps; i++) {
    t = t - alpha * f0/df(t);
    f0 = f(t);
    if(fabs(f0) < _tolerance && t>=min && t<=max) {
      result = t;
      return true;
    }
    else if(t > min-(max-min)/2 && t < max+(max-min)/2) {
      continue;
    }
    else {
      alpha *= .5;
      t = min;
      if(alpha < 1./8.) {
        return false;
      }
    }
  }

  if(t > min && t < max) {
    result = t;
  }
  else {
    result = (max+min)/2;
  }
  return false;
}
