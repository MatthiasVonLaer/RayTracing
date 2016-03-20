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

