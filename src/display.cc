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

#include "display.h"

#include "mpi_manager.h"
#include "utilities.h"

#include <iostream>

using namespace std;

Display::Display() :
  _display_progress(true),
  _display_summary(true),
  _summary_time(0),
  _summary_mpix(0),
  _summary_pics(0),
  _timer_running(false)
{
}

void Display::parse(const string &command, istream &stream)
{
  if(command == "progress") {
    stream >> _display_progress;
  }
  else if(command == "summary") {
    stream >> _display_summary;
  }
  else {
    parser_error_unknown_command(command);
  }
}

void Display::start_timer()
{
  if(_timer_running) {
    display_warning("Timer is already running.");
  }
  _timer_running = true;
  _t0 = get_time();
}

void Display::stop_timer()
{
  if(!_timer_running) {
    display_warning("Timer is already stopped.");
  }
  _timer_running = false;
  _summary_time += 1.e-6 * ( get_time() - _t0 );
}

void Display::add_mpix(int n)
{
  _summary_mpix += 1.e-6 * n;
  _summary_pics++;
}

void Display::progress(const string &name, double progress)
{
  //checking if display is necessary
  if(!_display_progress)
    return;
  if(_name == name && is_greater(_progress + 1./_width_bar, progress))
    return;

  _name = name;
  _progress = progress;

  cout << "\r" << name;
  for(int i=name.size(); i<_width_text; i++)
    cout << ' ';
  cout << '|';
  for(int i=0; i<_width_bar; i++) {
    if(i < progress*_width_bar)
      cout << '-';
    else
      cout << ' ';
  }
  cout << '|';
  if(progress > 0) {
    cout << " " << int(progress * 100) << "%";
  }
  cout << "     ";
  cout.flush();
}

void Display::summary() const
{
  if(!_display_summary) {
    cout << endl;
    return;
  }

  cout.precision(2);
  cout << std::fixed;
  cout << "\r";
  cout << "Pictures: " << _summary_pics << "     ";
  cout << "Nodes: " << mpi().size() << "     ";
  cout << "Total: " << _summary_mpix << " MPix     ";
  cout << "Time: " << _summary_time << " s     ";
  cout << "Speed: " << _summary_mpix/_summary_time << " MPix/s" << endl;
}

