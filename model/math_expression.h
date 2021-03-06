//  A raytracer program for rendering photorealistic images
//  Copyright 2016, 2018 Matthias von Laer
//
//  This program is released under the GNU GPLv3.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "math_operation.h"
#include "vector.h"

#include <string>
#include <vector>

class MathExpression
{
private:
  std::vector<std::string> _reverse_polish;

  mutable double _x, _y, _z;
  const double *_result;
  std::vector< std::unique_ptr<MathOperation> > _operations;
  std::vector< std::unique_ptr<const double> > _scalars;

public:
  MathExpression();
  MathExpression(const std::string &expr);
  MathExpression(const std::vector<std::string> &rpn);
  MathExpression(const MathExpression&); //not implemented

public:
  MathExpression& operator=(const MathExpression &f);
private:
  void set(const std::string &expr);
  void set(const std::vector<std::string> &rpn);
  void clear();
  void split(std::string expr, std::vector<std::string> &infix) const;
  void shunting_yard(const std::vector<std::string> &input, std::vector<std::string> &output) const;
  std::vector<std::string> simplify(std::vector<std::string> &rpn) const;

private:
  void initialize();
  const double* initialize_recursion(std::vector<std::string> &rpn);

public:
  double operator()(double x=0, double y=0, double z=0) const;
  double operator()(const Vector &v) const;

public:
  MathExpression dx() const;
  MathExpression dy() const;
  MathExpression dz() const;
private:
  std::vector<std::string> d(const std::string &var, std::vector<std::string> rpn) const;
  std::vector<std::string> get_argument(std::vector<std::string> &rpn) const;
  void concat(std::vector<std::string> &v1, const std::vector<std::string> &v2) const;

public:
  const std::vector< std::string >& reverse_polish_notation() const {return _reverse_polish;}
  std::string str() const;
};

