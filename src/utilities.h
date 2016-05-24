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

#pragma once

#include <complex>
#include <memory>
#include <string>

constexpr double PI = 3.141592653589793238;
constexpr std::complex<double> I(0.0, 1.0);
constexpr double TOL = 1.e-8;

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



#if __cplusplus == 201402L
using std::make_unique;
#else
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif
