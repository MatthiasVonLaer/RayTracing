#pragma once

#include <complex>
#include <map>

#include "polygon.h"

class Lens
{
public:
  Lens();
  ~Lens();
  double blur_diameter(double distance) const;
  Polygon& aperture_shape(int radius);
  std::complex<double> diffraction_pattern(double x_0, double y_0);

  double aperture() const           {return _aperture;}
  int    blades() const             {return _blades;}
  double focus() const              {return _focus;}
  double focal_length() const       {return _focal_length;}

  void set_aperture(double d)       {_aperture = d;     clear_diffraction_pattern();}
  void set_blades(int i)            {_blades = i;       clear_diffraction_pattern();  clear_aperture_shapes();}
  void set_focus(double d)          {_focus = d;}
  void set_focal_length(double d)   {_focal_length = d; clear_diffraction_pattern();}

private:
  void clear_aperture_shapes();
  void clear_diffraction_pattern();
  std::complex<double> integrate_diffraction_pattern(double x_0, double y_0);
  double r(double x_0, double y_0, double x, double y) const;

private:
  double _aperture;
  double _focus;
  double _focal_length;
  int _blades;

  std::map < int, Polygon* > _aperture_shapes;
  std::map < double, std::map < double, std::complex < double > > > _diffraction_pattern;

  const double _lambda;
  const int _integration_nodes_radius;
};
