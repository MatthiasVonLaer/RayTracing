#pragma once

#include <complex>
#include <map>

#include "polygon.h"

class Lens
{
private:
  const double _wavelength_light;
  const int _integration_nodes_radius;

  std::map < int, Polygon* > _aperture_shapes;
  std::map < double, std::map < double, std::complex < double > > > _diffraction_pattern;

  double _aperture;
  double _focus;
  double _focal_length;
  int _blades;


public:
  Lens();
  ~Lens();
  double blur_diameter(double distance) const;

public:
  Polygon& aperture_shape(int radius);
private:
  void clear_aperture_shapes();

public:
  std::complex<double> diffraction_pattern(double x_0, double y_0);
private:
  std::complex<double> integrate_diffraction_pattern(double x_0, double y_0);
  double r(double x_0, double y_0, double x, double y) const;
  void clear_diffraction_pattern();

public:
  double aperture() const           {return _aperture;}
  int    blades() const             {return _blades;}
  double focus() const              {return _focus;}
  double focal_length() const       {return _focal_length;}

  void set_aperture(double d)       {_aperture = d;     clear_diffraction_pattern();}
  void set_blades(int i)            {_blades = i;       clear_diffraction_pattern();  clear_aperture_shapes();}
  void set_focus(double d)          {_focus = d;}
  void set_focal_length(double d)   {_focal_length = d; clear_diffraction_pattern();}


};
