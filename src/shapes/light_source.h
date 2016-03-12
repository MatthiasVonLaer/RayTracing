#pragma once

#include <istream>

#include "ball.h"
#include "light_beam.h"
#include "vector.h"

class LightSource : public Ball
{
public:
  LightSource(const Composition *parent);
  virtual void parse(const std::string &command, std::istream &in);

  const LightBeam& beam() const      {return _beam;}

private:
  LightBeam _beam;
};
