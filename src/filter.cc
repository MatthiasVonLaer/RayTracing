#include "filter.h"

using namespace std;

istream& operator>>(istream &in, Filter &filter)
{
  double r, g, b;
  in >> r >> g >> b;
  filter = Filter(r, g, b);
}
