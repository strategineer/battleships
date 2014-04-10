#pragma once
#include "SDL.h"
/* Class storing various SDL_Color(s) struct. */
class Colors
{
public:
  static const SDL_Color BLACK;
  static const SDL_Color WHITE;
  static const SDL_Color RED;
  static const SDL_Color BLUE;
  static const SDL_Color GREEN;
private:
  Colors();
  ~Colors();
};

