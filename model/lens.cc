//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "lens.h"

#include "utilities.h"

#include <math.h>

#include <memory>

using namespace std;

static constexpr double s_wavelength_light = 550e-9;
static constexpr int s_integration_nodes_radius = 250;

Lens::Lens() :
  _focal_length      (.035),
  _aperture          (5.6),
  _blades            (0)
{
}

double Lens::blur_diameter(double distance) const
{
  const double blur_at_distance = fabs(distance - _focus) * (_focal_length) / _aperture / _focus;
  const double blur_on_film = blur_at_distance * (_focal_length) / distance;
  return blur_on_film;
}

Polygon& Lens::aperture_shape(int radius)
{
  if(!_aperture_shapes.count(radius)) {
    if(_blades == 0) {
      _aperture_shapes[radius] = make_unique<Polygon>(6*radius, radius);
    }
    else {
      _aperture_shapes[radius] = make_unique<Polygon>(_blades, radius);
    }
  }
  return *(_aperture_shapes[radius]);
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
  Polygon aperture = aperture_shape(s_integration_nodes_radius);
  const double ds = _focal_length / _aperture / (2*s_integration_nodes_radius);
  complex<double> integral = 0;
  for(int n=0; n<aperture.size(); n++) {
    double x = aperture.x(n) * ds; 
    double y = aperture.y(n) * ds;
    integral += exp( - 2 * PI * I / s_wavelength_light
              * (r(0, 0, x, y) - r(x_0, y_0, x, y)) ) * r(0, 0, x, y) / r(x_0, y_0, x, y);
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

