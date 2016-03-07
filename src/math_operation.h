#pragma once

#include <string>
#include <vector>
#include <assert.h>

class MathOperation
{
public:
  MathOperation(const std::string &pattern);

  void calculate();

  void set_arg(int i, const double* d)  {assert(i<_args.size()); _args[i] = d;}

  int number_args() const               {return _args.size();}
  const double* result() const          {return &_result;}

private:
  std::vector<const double*> _args;
  double _result;
  double (*_func1)(double);
  double (*_func2)(double, double);
};

