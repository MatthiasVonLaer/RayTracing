#include <iostream>
#include <math.h>

#include "root_finder.h"
#include "utilities.h"

using namespace std;

RootFinder::RootFinder() :
  _tolerance(1e-14),
  _min_step(0.1),
  _newton_steps(15),
  _nested_intervals_steps(15)
{
}

bool RootFinder::find(double (*f)(double), double (*df)(double), double lipschitz, double min, double max, double &result) const
{
  double t0 = min;
  double t1 = min;
  double f1 = f(t1);
  double sign_start = f1/fabs(f1);

  while(is_smaller(t1, max)) {
    t0 = t1;
    if(fabs(f1/lipschitz) > _min_step*(max - min)) {
      t1 += fabs(f1 / lipschitz);
    }
    else {
      t1 += _min_step * (max - min);
    }

    if(t1 > max) {
      t1 = max;
    }
    f1 = f(t1);

    if(sign_start*f1 < _tolerance) {
      if(newton(f, df, t0, t1, _newton_steps, result)) {
        return true;
      }
      if(nested_intervals(f, t0, t1, _nested_intervals_steps, result)) {
        return true;
      }

      display_warning("Rootfinder: unable to locate root in desired accuracy");
      result = (t0 + t1) / 2;
      return true;
    }
  }
  return false;
}

bool RootFinder::nested_intervals(double (*f)(double), double t0, double t1, int steps, double &result) const
{
  double f0 = f(t0);
  double f1 = f(t1);
  double sign_start = f0/fabs(f0);
  
  for(int i=0; i<steps; i++) {
    if(fabs(f0) < _tolerance && fabs(f0) < fabs(f1)) {
      result = t0;
      return true;
    }
    else if(fabs(f1) < _tolerance) {
      result = t1;
      return true;
    }

    double center = (t0+t1)/2;
    double fc = f(center);
    if(fabs(fc) < _tolerance) {
      result = center;
      return true;
    }
    if(sign_start*fc > 0) {
      t0 = center;
    }
    else {
      t1 = center;
    }
  }
  return false;
}

bool RootFinder::newton(double (*f)(double), double (*df)(double), double min, double max, int steps, double &result) const
{
  double alpha=1;
  double t = min;
  double f1, f0;

  f0 = f(t);
  if(fabs(f0) < _tolerance) {
    result = t;
    return true;
  }
  
  for(int i=0; i<steps; i++) {
    t = t - alpha * f0/df(t);
    f0 = f(t);
    if(fabs(f0) < _tolerance && t>=min && t<=max) {
      result = t;
      return true;
    }
    else if(t > min-(max-min)/4 && t < max+(max-min)/4) {
      continue;
    }
    else {
      alpha *= .5;
      if(alpha < 1./8.) {
        return false;
      }
    }
  }
  return false;
}
