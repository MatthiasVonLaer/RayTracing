#pragma once

class RootFinder 
{
private:
  const int _newton_steps;
  const int _nested_intervals_steps;
  const double _min_step;
  const double _tolerance;

public:
  RootFinder();
  bool find(double (*f)(double), double (*df)(double), double lipschitz, double min, double max, double &result) const;
  bool nested_intervals(double (*f)(double), double t0, double t1, int steps, double &result) const;
  bool newton(double (*f)(double), double (*df)(double), double a, double b, int steps, double &erg) const;
};
