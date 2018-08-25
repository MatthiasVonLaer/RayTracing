//  A raytracer program for rendering photorealistic images
//  Copyright 2016 Matthias von Laer
//
//  This program is released under the license GNU GPLv3.
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
