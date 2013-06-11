#include "rgb.hh"


Rgb::Rgb(int red, int green, int blue)
{
  red_ = red;
  blue_ = blue;
  green_ = green;
}

Rgb::Rgb()
{
  red_ = 0;
  blue_ = 0;
  green_ = 0;
}


Rgb::~Rgb()
{}



unsigned int Rgb::get_red()
{
  return red_;
}


unsigned int Rgb::get_green()
{
  return green_;
}


unsigned int Rgb::get_blue()
{
  return blue_;
}

void Rgb::set_red(unsigned int r)
{
  red_ = r;
}

void Rgb::set_green(unsigned int g)
{
  green_ = g;
}

void Rgb::set_blue(unsigned int b)
{
  blue_ = b;
}

void Rgb::set_rgb(unsigned int r, unsigned int g, unsigned int b)
{
  red_ = r;
  green_ = g;
  blue_ = b;
}

