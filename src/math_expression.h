#pragma once

#include <string>
#include <vector>

#include "math_operation.h"
#include "vector.h"

class MathExpression
{
public:
  MathExpression();
  MathExpression(const std::string &expr);
  MathExpression(const std::vector<std::string> &rpn);
  ~MathExpression();

  void set(const std::string &expr);
  void set(const std::vector<std::string> &rpn);
  void clear();
  void operator=(const MathExpression &f);

  double operator()(double x=0, double y=0, double z=0) const;
  double operator()(const Vector &v) const;
  MathExpression dx() const;
  MathExpression dy() const;
  MathExpression dz() const;

  const std::vector< std::string >& reverse_polish_notation() const {return _reverse_polish;}

  std::string str() const;

private:
  void initialize();
  const double* initialize_recursion(std::vector<std::string> &rpn);

  std::vector<std::string> d(const std::string &var, std::vector<std::string> rpn) const;
  std::vector<std::string> get_argument(std::vector<std::string> &rpn) const;
  void concat(std::vector<std::string> &v1, const std::vector<std::string> &v2) const;

  std::vector<std::string> simplify(std::vector<std::string> &rpn) const;

  void split(const std::string &expr, std::vector<std::string> &infix) const;
  void shunting_yard(const std::vector<std::string> &input, std::vector<std::string> &output) const;

  static int precedence(const std::string &op);

  static bool is_operator(const std::string &str);
  static bool is_number(const std::string &str);
  static bool is_function(const std::string &str);
  static bool is_variable(const std::string &str);

  static bool is_operator(char c) {return is_operator((std::string(1, c)));}
  static bool is_number(char c)   {return is_number((  std::string(1, c)));}
  static bool is_function(char c) {return is_function((std::string(1, c)));}
  static bool is_variable(char c) {return is_variable((std::string(1, c)));}

private:
  std::vector<std::string> _reverse_polish;

  mutable double _x, _y, _z;
  const double *_result;
  std::vector<MathOperation*> _operations;
  std::vector<const double*> _scalars;
private:
  static const std::string operator_chars;
  static const std::string number_chars; 
  static const std::string function_chars; 
};

