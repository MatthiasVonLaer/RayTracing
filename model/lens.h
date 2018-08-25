//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "polygon.h"

#include <complex>
#include <map>
#include <memory>

class Lens
{
private:
  std::map < int, std::unique_ptr<Polygon> > _aperture_shapes;
  std::map < double, std::map < double, std::complex < double > > > _diffraction_pattern;

  double _aperture;
  double _focus;
  double _focal_length;
  int _blades;


public:
  Lens();
  double blur_diameter(double distance) const;

public:
  Polygon& aperture_shape(int radius);

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
  void set_blades(int i)            {_blades = i;       clear_diffraction_pattern();  _aperture_shapes.clear();}
  void set_focus(double d)          {_focus = d;}
  void set_focal_length(double d)   {_focal_length = d; clear_diffraction_pattern();}
};
