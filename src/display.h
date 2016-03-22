//  Copyright 2016 Matthias von Laer
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <istream>
#include <string>

class Display
{
private:
  const int _width_text;
  const int _width_bar;

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
