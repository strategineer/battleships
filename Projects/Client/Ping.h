#pragma once
#include "SDL.h"
#include "Point.h"
class Ping
{
public:
  Ping(const Point p, int t);
  ~Ping();

  bool has_expired() const;
  const Point & get_location() const;
private:
  const Point location;

  /* Ticks when the message was created. */
  Uint32 start_ticks;
  /* Time in ms to display the message for. */
  int time_in_ms;
};

