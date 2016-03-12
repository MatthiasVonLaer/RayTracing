#pragma once

class RootFinder {
public:
  RootFinder();
  bool find(double (*f)(double), double (*df)(double), double lipschitz, double min, double max, double &result) const;
  bool nested_intervals(double (*f)(double), double t0, double t1, int steps, double &result) const;
  bool newton(double (*f)(double), double (*df)(double), double a, double b, int steps, double &erg) const;

private:
  double _min_step;
  double _tolerance;
  const int _newton_steps;
  const int _nested_intervals_steps;
};
