#pragma once
#include <complex>
#include <string>

const double PI = 3.141592653589793238;
const std::complex<double> I(0.0, 1.0);
const double TOL = 1.e-8;

bool is_equal(double d1, double d2, double epsilon=TOL);
bool is_greater(double d1, double d2, double epsilon=TOL);
bool is_greater_or_equal(double d1, double d2, double epsilon=TOL);
bool is_smaller(double d1, double d2, double epsilon=TOL);
bool is_smaller_or_equal(double d1, double d2, double epsilon=TOL);
double f_rand(double f_min, double f_max);
long get_time();
bool file_exists(const std::string &path);
void display_warning(const std::string &text);
void display_error(const std::string &text);
void parser_error_unknown_command(const std::string &command);
void parser_assert_command(const std::string &command, const std::string &expected_command);
