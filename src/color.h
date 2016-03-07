#pragma once

#include <istream>
#include <string>

#include <QColor>

class Color
{
public:
  Color()                                 {_r=0; _g=0; _b=0;}
  Color(int r_in, int g_in, int b_in)     {_r=r_in; _g=g_in; _b=b_in;}
  Color(const QRgb &color)                {_r=qRed(color); _g=qGreen(color); _b=qBlue(color);}

  //void set_red(char d)                    {_r = d;}
  //void set_green(char d)                  {_g = d;}
  //void set_blue(char d)                   {_b = d;}

  int red() const                         {return _r;}
  int green() const                       {return _g;}
  int blue() const                        {return _b;}

  std::string str();
private:
  int _r;
  int _g;
  int _b;
};

std::istream& operator>>(std::istream &in, Color &color);
