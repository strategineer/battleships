#include "Ping.h"


Ping::Ping(const Point p, int t) : location(p), time_in_ms(t)
{
  start_ticks = SDL_GetTicks();
}


Ping::~Ping()
{}

bool Ping::has_expired() const
{
  return ( SDL_GetTicks() - start_ticks ) > time_in_ms;
}

const Point & Ping::get_location() const
{
  return location;
}
