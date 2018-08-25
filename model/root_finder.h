//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <functional>

class RootFinder 
{
private:
  const int _newton_steps;
  const int _nested_intervals_steps;
  const double _min_step;
  const double _tolerance;

public:
  using Function = std::function<double(double)>;

  RootFinder();
  bool find(Function f, Function df, double min, double max, double &result) const;
  bool nested_intervals(Function f, double t0, double t1, int steps, double &result) const;
  bool newton(Function f, Function df, double a, double b, int steps, double &erg) const;
};
