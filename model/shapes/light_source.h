//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "ball.h"
#include "../light_beam.h"
#include "../vector.h"

#include <istream>

class LightSource : public Ball
{
public:
  LightSource(Composition *parent);

  virtual void parse(const std::string &command, std::istream &in) override;

  const LightBeam& beam() const      {return _beam;}

private:
  LightBeam _beam;
};
