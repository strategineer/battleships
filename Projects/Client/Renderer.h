#pragma once
#include "SDL.h"
/* Wrapper for an SDL_Renderer*. */
class Renderer
{
public:
  Renderer(SDL_Renderer* rend);
  ~Renderer();
  SDL_Renderer* get_sdl_renderer();
private:
  SDL_Renderer* sdl_renderer;
};

