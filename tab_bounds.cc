#include "tab_bounds.hh"


Tab_bounds::Tab_bounds()
{
  first_exchange_ = true;
}

Tab_bounds::~Tab_bounds()
{
}



int Tab_bounds::get_x()
{
  return x_;
}


int Tab_bounds::get_y()
{
  return y_;
}



void Tab_bounds::set_x(int x)
{
  x_ = x;
}


void Tab_bounds::set_y(int y)
{
  y_ = y;
}


bool Tab_bounds::first_exchange()
{
  return first_exchange_;
}


void Tab_bounds::turn_off_ex()
{
  first_exchange_ = false;
}
