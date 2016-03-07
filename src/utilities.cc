#include "utilities.h"
#include "vector.h"

#include <math.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>
#include <QString>

using namespace std;

bool is_equal(double a, double b, double epsilon)
{
  return fabs(a - b) <= epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_greater(double a, double b, double epsilon)
{
  return a - b >= epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_greater_or_equal(double a, double b, double epsilon)
{
  return a - b >= -epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_smaller(double a, double b, double epsilon)
{
  return b - a >= epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

bool is_smaller_or_equal(double a, double b, double epsilon)
{
  return b - a >= -epsilon;// * (fabs(a) < fabs(b) ? fabs(b) : fabs(a));
}

double round_down(double d)
{
  if(d >= 0)
    return int(d);
  else
    return int(d-1);
}

double f_rand(double f_min, double f_max)
{
  double f = (double)rand() / RAND_MAX;
  return f_min + f * (f_max - f_min);
}

long get_time()
{
  timeval val;
  gettimeofday(&val, 0);
  return 1000000 * val.tv_sec + val.tv_usec;
}

bool file_exists(const string &path)
{
  ifstream in(path.c_str());
  return in.is_open();
}

void display_warning(const std::string &text)
{
  cerr << endl << "\033[0;33m" << text << "\033[0m" << endl;
}

void display_error(const std::string &text)
{
  cerr << endl << "\033[0;31m" << text << "\033[0m" << endl;
  exit(1);
}

void parser_error_unknown_command(const std::string &command)
{
  display_error("Parser Error: Command " + command + " not known.");
}

void parser_assert_command(const std::string &command, const std::string &expected_command)
{
  if(command != expected_command) {
    display_error("Parser Error: Got keyword " + command + " instead of the expected keyword " + expected_command + ".");
  }
}
