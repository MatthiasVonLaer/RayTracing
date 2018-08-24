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
