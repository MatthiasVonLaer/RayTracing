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

#include <string>
#include <vector>
#include <assert.h>

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

