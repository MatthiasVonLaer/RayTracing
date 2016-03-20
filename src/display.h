#pragma once

#include <istream>
#include <string>

class Display
{
private:
  const int _width_text;
  const int _width_bar;

  bool _display_progress;
  bool _display_summary;

  std::string _name;
  double _progress;

  double _summary_time;
  double _summary_mpix;
  int _summary_pics;
  long _t0;

  bool _timer_running;

public:
  Display();
  void parse(const std::string &command, std::istream &stream);

  void start_timer();
  void stop_timer();
  void add_mpix(int n);

  void progress(const std::string &name, double progress);
  void summary() const;
};
