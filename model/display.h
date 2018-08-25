//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <istream>
#include <string>

class Display
{
private:
  bool _display_progress;
  bool _display_summary;

  std::string _name;
  double _progress;

  double _summary_time;
  double _summary_mpix;
  int _summary_pics;
  long _t0;

  bool _timer_running;

public:
  Display();
  void parse(const std::string &command, std::istream &stream);

  void start_timer();
  void stop_timer();
  void add_mpix(int n);

  void progress(const std::string &name, double progress);
  void summary() const;
};
