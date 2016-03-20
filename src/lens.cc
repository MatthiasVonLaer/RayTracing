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

#include "lens.h"
#include "utilities.h"

using namespace std;

Lens::Lens() :
  _wavelength_light  (800e-9),
  _integration_nodes_radius(250),
  _focal_length      (.035),
  _aperture          (5.6),
  _blades            (0)
{
}

Lens::~Lens()
{
  clear_aperture_shapes();
}

double Lens::blur_diameter(double distance) const
{
  double blur_at_distance = fabs(distance - _focus) * (_focal_length) / _aperture / _focus;
  double blur_on_film = blur_at_distance * (_focal_length) / distance;
  return blur_on_film;
}

Polygon& Lens::aperture_shape(int radius)
{
  if(!_aperture_shapes.count(radius)) {
    if(_blades == 0) {
      _aperture_shapes[radius] = new Polygon(6*radius, radius);
    }
    else {
      _aperture_shapes[radius] = new Polygon(_blades, radius);
    }
  }
  return *(_aperture_shapes[radius]);
}

void Lens::clear_aperture_shapes()
{
  for(map<int, Polygon*>::iterator it = _aperture_shapes.begin(); it != _aperture_shapes.end(); it++) {
    delete it->second;
  }
  _aperture_shapes.clear();
}

complex<double> Lens::diffraction_pattern(double x_0, double y_0)
{
  if(!_diffraction_pattern.count(x_0) || !_diffraction_pattern[x_0].count(y_0)) {
    _diffraction_pattern[x_0][y_0] = integrate_diffraction_pattern(x_0, y_0);
  }
  return _diffraction_pattern[x_0][y_0];
}

complex<double> Lens::integrate_diffraction_pattern(double x_0, double y_0)
{
  Polygon aperture = aperture_shape(_integration_nodes_radius);
  double ds = _focal_length / _aperture / (2*_integration_nodes_radius);
  complex<double> integral = 0;
  for(int n=0; n<aperture.size(); n++) {
    double x = aperture.x(n) * ds; 
    double y = aperture.y(n) * ds;
    //integral += exp( - 2 * PI * I / _wavelength_light * (- r(x_0, y_0, x, y)) ) / r(x_0, y_0, x, y);
    integral += exp( - 2 * PI * I / _wavelength_light * (r(0, 0, x, y) - r(x_0, y_0, x, y)) ) * r(0, 0, x, y) / r(x_0, y_0, x, y);
  }
  integral /= aperture.size();
  return integral;
}

double Lens::r(double x_0, double y_0, double x, double y) const
{
  return sqrt( pow(x-x_0, 2) + pow(y-y_0, 2) + pow(_focal_length, 2) );
}

void Lens::clear_diffraction_pattern()
{
  _diffraction_pattern.clear();
}

