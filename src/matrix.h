#pragma once

#include <string>

#include "vector.h"

class Matrix
{
public:
  Matrix(double a00, double a01, double a02, double a10, double a11, double a12, double a20, double a21, double a22);
  Matrix(double A[3][3]);
  Matrix(const Vector &v0, const Vector &v1, const Vector &v2);
  Matrix(double d);
  Matrix();

  double det() const;
  Matrix inv() const;

  Matrix operator*(const Matrix &m) const;
  Vector operator*(const Vector &v) const;
  Matrix operator*(double d) const;
  Matrix operator/(double d) const;
  double operator()(int i, int j) const;

  std::string str() const;

private:
  double cofactor(int i, int j) const;

private:
  double _a[3][3];
};
