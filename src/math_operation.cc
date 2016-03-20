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

#include <iostream>
#include <math.h>

#include "math_operation.h"
#include "utilities.h"

using namespace std;

double add(double a, double b)       {return a+b;}
double subtract(double a, double b)  {return a-b;}
double multiply(double a, double b)  {return a*b;}
double divide(double a, double b)    {return a/b;}

MathOperation::MathOperation(const string &pattern) :
  _func1(0),
  _func2(0)
{
  if(pattern == "+") {
    _func2 = &add;
  }
  else if(pattern == "-") {
    _func2 = &subtract;
  }
  else if(pattern == "*") {
    _func2 = &multiply;
  }
  else if(pattern == "/") {
    _func2 = &divide;
  }
  else if(pattern == "^") {
    _func2 = &pow;
  }
  else if(pattern == "cos") {
    _func1 = &cos;
  }
  else if(pattern == "exp") {
    _func1 = &exp;
  }
  else if(pattern == "log") {
    _func1 = &log;
  }
  else if(pattern == "sin") {
    _func1 = &sin;
  }
  else if(pattern == "sqrt") {
    _func1 = &sqrt;
  }
  else {
    display_error("MathOperation: Pattern not known: " + pattern);
  }

  if(_func1) {
    _args.resize(1);
  }
  else if(_func2) {
    _args.resize(2);
  }
  else {
    display_error("MathOperation: No function defined.");
  }
}

void MathOperation::calculate()
{
  if(_args.size() == 1) {
    _result = (*_func1)(*(_args[0]));
  }
  else if(_args.size() == 2) {
    _result = (*_func2)(*(_args[0]), *(_args[1]));
  }
  else {
    assert(0);
    display_error("MathOperation: number_args should be 1 or 2.");
  }
}
