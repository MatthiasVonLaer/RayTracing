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
#include <sstream>
#include <math.h>

#include "polygon.h"
#include "utilities.h"

using namespace std;

Polygon::Polygon(int number_vertices, int radius) :
  _number_vertices(number_vertices),
  _radius(radius),
  _diameter(2*radius+1)
{
  if(_radius < 0) {
    display_error("Polygon: Negativ radius.");
  }
  if(_number_vertices < 3) {
    display_error("Polygon: Min 3 vertices.");
  }

  _pixelmatrix.resize(_diameter);
  for(int i=0; i<_pixelmatrix.size(); i++) {
    _pixelmatrix[i].resize(_diameter);
    for(int j=0; j<_pixelmatrix[i].size(); j++) {
      _pixelmatrix[i][j] = false;
    }
  }

  fill_matrix();
  fill_vector();
}

void Polygon::fill_matrix()
{
  vector <int> vertex_x;
  vector <int> vertex_y;

  for(int i=0; 2*i <= _number_vertices; i++) {

    double phi  =  2*PI * i / _number_vertices;

    vertex_x.push_back( round( _radius * (1 - sin(phi)) ) );
    vertex_y.push_back( round( _radius * (1 - cos(phi)) ) );
    _pixelmatrix[vertex_x.back()][vertex_y.back()] = true;
  }

  if(_number_vertices % 2) {
    vertex_x.push_back( 2*_radius - vertex_x.back() );  
    vertex_y.push_back(             vertex_y.back() );  
  }

  for(int i=0; i<vertex_x.size()-1; i++) {
    const double dist = sqrt(pow(vertex_x[i] - vertex_x[i+1], 2) + pow(vertex_y[i] - vertex_y[i+1], 2));
    if(is_equal(dist, 0)) {
      break;
    }
    double x = vertex_x[i];
    double y = vertex_y[i];
    const double dx = (vertex_x[i+1] - vertex_x[i]) / dist;
    const double dy = (vertex_y[i+1] - vertex_y[i]) / dist;
    double ds = .5;
    for(double s=0; s < dist; s+=ds, x+=ds*dx, y+=ds*dy) {
      _pixelmatrix[ round(x) ][ round(y) ] = true;
    }
  }

  for(int j=0; j<_diameter; j++) {
    bool inside = false;
    for(int i=0; i<_diameter; i++) {
      if(i <= _radius) {
        if(_pixelmatrix[i][j]) {
          inside = true;
        }
        else if(inside) {
          _pixelmatrix[i][j] = true;
        }
      }
      else {
        _pixelmatrix[i][j] = _pixelmatrix[2*_radius-i][j];
      }
    }
  }
}

void Polygon::fill_vector()
{
  for(int j=0; j<_diameter; j++) {
    for(int i=0; i<_diameter; i++) {
      if(_pixelmatrix[i][j]) {
        _pixelvector_x.push_back(i-_radius);
        _pixelvector_y.push_back(j-_radius);
      }
    }
  }
}

string Polygon::str() const
{
  stringstream stream;
  for(int j=0; j<_pixelmatrix.size(); j++) {
    for(int i=0; i<_pixelmatrix[j].size(); i++) {
      if(_pixelmatrix[i][j]) {
        stream << " X";
      }
      else {
        stream << " .";
      }
    }
    stream << endl;
  }
  return stream.str();
}

