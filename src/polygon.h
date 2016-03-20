#pragma once

#include <string>
#include <vector>

class Polygon
{
private:
  const int _number_vertices;
  const int _radius;
  const int _diameter;

  std::vector < std::vector < bool > > _pixelmatrix;
  std::vector < int > _pixelvector_x;
  std::vector < int > _pixelvector_y;

public:
  Polygon(int number_vertices, int radius);

private:
  void fill_matrix();
  void fill_vector();

public:
  int x(int n) const {return _pixelvector_x[n];}
  int y(int n) const {return _pixelvector_y[n];}
  int size() const   {return _pixelvector_x.size();}

  std::string str() const;
};
