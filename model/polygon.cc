//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "polygon.h"

#include "utilities.h"

#include <math.h>

#include <iostream>
#include <sstream>

using namespace std;

Polygon::Polygon(int number_vertices, int radius) :
  _number_vertices(number_vertices),
  _radius(radius),
  _diameter(2*radius+1)
{
  if(_radius < 0) {
    throw std::runtime_error("Polygon: Negativ radius.");
  }
  if(_number_vertices < 3) {
    throw std::runtime_error("Polygon: Min 3 vertices.");
  }

  _pixelmatrix.resize(_diameter);
  for(auto& row : _pixelmatrix) {
    row.resize(_diameter);
    for(auto&& element : row) {//proxy-iterator
      element = false;
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
  for(const auto& row : _pixelmatrix) {
    for(const auto&& element : row) {//proxy-iterator
      if(element) {
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

