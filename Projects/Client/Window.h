#pragma once
#include "SDL.h"
class Window
{
public:
  Window(SDL_Window* wind);
  ~Window();
  SDL_Window* get_sdl_window();
private:
  SDL_Window* sdl_window;
};

