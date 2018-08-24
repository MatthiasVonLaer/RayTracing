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

#include "math_expression.h"

#include "utilities.h"

#include <math.h>

#include <iostream>
#include <sstream>

using namespace std;

static const string operator_chars  = "+-*/^()";
static const string number_chars    = "0123456789."; 
static const string function_chars  = "abcdefghijklmnopqrstuvwxyz";

static int precedence(const string &op)
{
  if(op == "(")
    return 1;
  if(op == ")")
    return 1;
  if(op == "-")
    return 2;
  if(op == "+")
    return 2;
  if(op == "*")
    return 3;
  if(op == "/")
    return 3;
  if(op == "^")
    return 4;
  throw std::logic_error("MathExpression: " + op + " is not a valid operator.");
}

static bool is_operator(const string &str)
{
  for(const auto& c : str)
    if(operator_chars.find(c) == string::npos)
      return false;
  return true;
}

static bool is_number(const string &str)
{
  if(str.size() == 1 && str[0] == '-')
    return false;

  int i = 0;
  if(str.size() && str[0] == '-')
    i = 1;

  for(; i<str.size(); i++)
    if(number_chars.find(str[i]) == string::npos)
      return false;
  return true;
}

static bool is_variable(const string &str)
{
  return str == "x" || str == "y" || str == "z";
}

static bool is_function_name(const string &str)
{
  if(is_variable(str))
    return false;
  for(const auto& c : str)
    if(function_chars.find(c) == string::npos)
      return false;
  return true;
}

static bool is_operator(char c)       {return is_operator((     string(1, c)));}
static bool is_number(char c)         {return is_number((       string(1, c)));}
static bool is_function_name(char c)  {return is_function_name((string(1, c)));}
static bool is_variable(char c)       {return is_variable((     string(1, c)));}



MathExpression::MathExpression()
{
  set("0");
}

MathExpression::MathExpression(const string &expr)
{
  set(expr);
}

MathExpression::MathExpression(const vector<string> &rpn)
{
  set(rpn);
}

MathExpression& MathExpression::operator=(const MathExpression &f)
{
  set(f.reverse_polish_notation());
  return *this;
}

void MathExpression::set(const std::string &expr)
{
  clear();

  vector<string> infix;

  split(expr, infix);
  shunting_yard(infix, _reverse_polish);
  _reverse_polish = simplify(_reverse_polish);
  initialize();
}

void MathExpression::set(const std::vector<std::string> &rpn)
{
  clear();

  _reverse_polish = rpn;
  _reverse_polish = simplify(_reverse_polish);
  initialize();
}

void MathExpression::clear()
{
  _scalars.clear();
  _operations.clear();
}

void MathExpression::split(string expr, vector<string> &infix) const
{
  for(int i=0; i<expr.size();) {
    if(expr[i] == ' ') {
      expr.erase( expr.begin() + i );
      continue; 
    }
    else if(is_operator( expr[i] )) {
      if(expr[i] == '-' && (i==0 || is_operator( expr[i-1] ))) {
        infix.push_back("-1");
        infix.push_back("*");
      }
      else {
        infix.push_back( expr.substr(i, 1) );
      }
      i++;
      continue;
    }
    else if(is_number( expr[i] )) {
      size_t j = expr.find_first_not_of( number_chars, i);
      infix.push_back( expr.substr(i, j-i) );
      i = j;
      continue;
    }
    else if(is_function_name( expr[i] ) || is_variable( expr[i] )) {
      size_t j = expr.find_first_not_of( function_chars, i);
      infix.push_back( expr.substr(i, j-i) );
      i = j;
      continue;
    }
    else {
      throw std::runtime_error("MathExpression, Parser Error: Sign " + expr.substr(i, 1) + " unknown");
    }
  }

  for(int i=infix.size()-1; i>0; i--) {
    if(!is_operator(infix[i][0]) && !is_operator(infix[i-1][0])) {
      infix.insert(infix.begin()+i, "*");
    }
  }
}

void MathExpression::shunting_yard(const vector<string> &input, vector<string> &output) const
{
  vector<string> stack;

  for(int i=0; i<input.size(); i++) {

    //for(int n=0; n<output.size(); n++)
      //cout << output[n] << " ";
    //cout << "   ";
    //for(int n=0; n<stack.size(); n++)
      //cout << stack[n] << " ";
    //cout << endl;

    if(is_number(input[i])) {
      output.push_back(input[i]);
    }

    else if(is_variable(input[i])) {
      output.push_back(input[i]);
    }
    else if(is_function_name(input[i])) {
      stack.push_back(input[i]);
    }

    else if(is_operator(input[i])) {
      if(input[i] == "(") {
        stack.push_back(input[i]);
      }
      else if(input[i] == ")") {
        int j;
        for(j=stack.size()-1; j>=0; j--) {
          if(stack[j] != "(") {
            output.push_back(stack.back());
            stack.pop_back();
          }
          else {
            stack.pop_back();
            if(j-1 >= 0 && is_function_name(stack.back())) {
              output.push_back(stack.back());
              stack.pop_back();
            }
            break;
          }
        }
        if(j < 0) {
          throw std::runtime_error("Misplaced paranthese.");
        }
      }
      else { //input[i] = +-*/^
        for(int j=stack.size()-1; j>=0; j--) {
          if(is_operator(stack[j]) && precedence(input[i]) <= precedence(stack[j])) {
            if(input[i] == "^" && stack[j] == "^") {
              throw std::runtime_error("Ambigous order of two ^");
            }
            output.push_back(stack.back());
            stack.pop_back();
          }
          else {
            break;
          }
        }
        stack.push_back(input[i]);
      }
    }

    else {
      throw std::runtime_error("MathExpression: Unexpected element: " + input[i]);
    }
  }

  for(int i=stack.size()-1; i>=0; i--) {
    output.push_back(stack.back());
    stack.pop_back();
  }
}

vector<string> MathExpression::simplify(vector<string> &rpn) const
{
  string pattern = rpn.back();
  rpn.pop_back();

  if(is_variable(pattern) || is_number(pattern)) {
    return vector<string>(1, pattern);
  }
  else {
    MathOperation op(pattern);
    int N = op.number_args();

    vector<string> op_args[ N ];
    double args_double[ N ];
    bool only_numbers_as_args = true;
    for(int i=N-1; i>=0; i--) {

      op_args[i] = get_argument(rpn);
      op_args[i] = simplify(op_args[i]);

      if(is_number(op_args[i].back())) {
        stringstream stream(op_args[i].back());
        stream >> args_double[i];
        op.set_arg(i, &(args_double[i]));
      }
      else {
        only_numbers_as_args = false;
      }

    }
    if(only_numbers_as_args) {
      op.calculate();
      stringstream stream;
      stream << *(op.result());
      return vector<string>(1, stream.str());
    }
    else if(pattern == "+" && is_number(op_args[0].back()) && args_double[0] == 0) {
      return op_args[1];
    }
    else if(pattern == "+" && is_number(op_args[1].back()) && args_double[1] == 0) {
      return op_args[0];
    }
    else if(pattern == "-" && is_number(op_args[0].back()) && args_double[0] == 0) {
      op_args[1].push_back("-1");
      op_args[1].push_back("*");
      return op_args[1];
    }
    else if(pattern == "-" && is_number(op_args[1].back()) && args_double[1] == 0) {
      return op_args[0];
    }
    else if(pattern == "*" && is_number(op_args[0].back()) && args_double[0] == 0) {
      return vector<string>(1, "0");
    }
    else if(pattern == "*" && is_number(op_args[1].back()) && args_double[1] == 0) {
      return vector<string>(1, "0");
    }
    else if(pattern == "*" && is_number(op_args[0].back()) && args_double[0] == 1) {
      return op_args[1];
    }
    else if(pattern == "*" && is_number(op_args[1].back()) && args_double[1] == 1) {
      return op_args[0];
    }
    else if(pattern == "/" && is_number(op_args[1].back()) && args_double[1] == 1) {
      return op_args[0];
    }
    else if(pattern == "^" && is_number(op_args[1].back()) && args_double[1] == 1) {
      return op_args[0];
    }
    else {
      vector<string> argument;
      for(int i=0; i<N; i++) {
        concat(argument, op_args[i]);
      }
      argument.push_back(pattern);

      return argument;
    }
  }
}

void MathExpression::initialize()
{
  vector<string> rpn = _reverse_polish;
  _result = initialize_recursion(rpn);
}

const double* MathExpression::initialize_recursion(vector<string> &rpn)
{
  if(!rpn.size()) {
    throw std::runtime_error("MathExpression, init: Unexpected end of expression.");
  }

  string pattern = rpn.back();
  rpn.pop_back();

  if(pattern == "x") {
    return &_x;
  }
  else if(pattern == "y") {
    return &_y;
  }
  else if(pattern == "z") {
    return &_z;
  }
  else if(is_number(pattern)) {
    double d;
    stringstream stream(pattern);
    stream >> d;
    _scalars.push_back( make_unique<double>(d) );

    return _scalars.back().get();
  }
  else {
    _operations.push_back( make_unique<MathOperation>(pattern) );

    int n = _operations.size()-1;
    for(int i=_operations[n]->number_args()-1; i>=0; i--) {
      _operations[n]->set_arg(i, initialize_recursion(rpn));
    }

    return _operations[n]->result();
  }
}

double MathExpression::operator()(double x, double y, double z) const
{
  _x = x;
  _y = y;
  _z = z;
  for(int i=_operations.size()-1; i>=0; i--) {
    _operations[i]->calculate();
  }
  return *_result;
}

double MathExpression::operator()(const Vector &v) const
{
  return (*this)(v.x(), v.y(), v.z());
}

MathExpression MathExpression::dx() const
{
  vector<string> rpn = _reverse_polish;
  return d("x", rpn);
}

MathExpression MathExpression::dy() const
{
  vector<string> rpn = _reverse_polish;
  return d("y", rpn);
}

MathExpression MathExpression::dz() const
{
  vector<string> rpn = _reverse_polish;
  return d("z", rpn);
}

vector<string> MathExpression::d(const string &var, vector<string> rpn) const
{
  string pattern = rpn.back();
  rpn.pop_back();

  if(pattern == var) {
    return vector<string>(1, "1");
  }
  else if(is_variable(pattern) || is_number(pattern)) {
    return vector<string>(1, "0");
  }
  else {
    MathOperation op(pattern);
    vector< vector<string> > op_args;
    for(int i=0; i<op.number_args(); i++) {
      op_args.insert(op_args.begin(), get_argument(rpn));
    }

    vector<string> derivative;
    if(pattern == "+") {
      concat(derivative, d(var, op_args[0]));
      concat(derivative, d(var, op_args[1]));
      derivative.push_back("+");
    }
    else if(pattern == "-") {
      concat(derivative, d(var, op_args[0]));
      concat(derivative, d(var, op_args[1]));
      derivative.push_back("-");
    }
    else if(pattern == "*") {
      concat(derivative, d(var, op_args[0]));
      concat(derivative,        op_args[1]);
      derivative.push_back("*");
      concat(derivative,        op_args[0]);
      concat(derivative, d(var, op_args[1]));
      derivative.push_back("*");
      derivative.push_back("+");
    }
    else if(pattern == "/") {
      concat(derivative, d(var, op_args[0]));
      concat(derivative,        op_args[1]);
      derivative.push_back("*");
      concat(derivative,        op_args[0]);
      concat(derivative, d(var, op_args[1]));
      derivative.push_back("*");
      derivative.push_back("-");
      concat(derivative,        op_args[1]);
      concat(derivative,        op_args[1]);
      derivative.push_back("*");
      derivative.push_back("/");
    }
    else if(pattern == "^") {
      concat(derivative,        op_args[0]);
      concat(derivative,        op_args[1]);
      derivative.push_back("1");
      derivative.push_back("-");
      derivative.push_back("^");
      concat(derivative,        op_args[1]);
      concat(derivative, d(var, op_args[0]));
      derivative.push_back("*");
      derivative.push_back("*");

      concat(derivative,        op_args[0]);
      concat(derivative,        op_args[1]);
      derivative.push_back("^");
      concat(derivative,        op_args[0]);
      derivative.push_back("log");
      concat(derivative, d(var, op_args[1]));
      derivative.push_back("*");
      derivative.push_back("*");

      derivative.push_back("+");
    }
    else if(pattern == "exp") {
      concat(derivative,        op_args[0]);
      derivative.push_back("exp");
      concat(derivative, d(var, op_args[0]));
      derivative.push_back("*");
    }
    else if(pattern == "sin") {
      concat(derivative,        op_args[0]);
      derivative.push_back("cos");
      concat(derivative, d(var, op_args[0]));
      derivative.push_back("*");
    }
    else if(pattern == "cos") {
      concat(derivative,        op_args[0]);
      derivative.push_back("sin");
      concat(derivative, d(var, op_args[0]));
      derivative.push_back("-1");
      derivative.push_back("*");
      derivative.push_back("*");
    }
    else if(pattern == "log") {
      concat(derivative, d(var, op_args[0]));
      concat(derivative,        op_args[0]);
      derivative.push_back("/");
    }
    else if(pattern == "sqrt") {
      concat(derivative, d(var, op_args[0]));
      concat(derivative,        op_args[0]);
      derivative.push_back("sqrt");
      derivative.push_back("2");
      derivative.push_back("*");
      derivative.push_back("/");
    }
    else {
      for(int i=0; i<op_args.size(); i++) {
        concat(derivative, (d(var, op_args[i])));
      }
      derivative.push_back(pattern);
    }

    return derivative;
  }
}

vector<string> MathExpression::get_argument(vector<string> &rpn) const
{
  string pattern = rpn.back();
  rpn.pop_back();

  if(is_variable(pattern) || is_number(pattern)) {
    return vector<string>(1, pattern);
  }
  else {
    vector<string> argument;
    argument.push_back(pattern);

    MathOperation op(pattern);
    for(int i=0; i<op.number_args(); i++) {
      vector<string> op_arg = get_argument(rpn);
      argument.insert(argument.begin(), op_arg.begin(), op_arg.end());
    }
    return argument;
  }
}

void MathExpression::concat(vector<string> &v1, const vector<string> &v2) const
{
  v1.insert(v1.end(), v2.begin(), v2.end());
}

string MathExpression::str() const
{
  stringstream stream;
  for(int i=0; i<_reverse_polish.size(); i++) {
    stream << _reverse_polish[i] << " ";
  }
  return stream.str();
}

