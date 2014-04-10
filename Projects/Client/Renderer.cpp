#include "Renderer.h"


Renderer::Renderer(SDL_Renderer* rend) : sdl_renderer(rend)
{}


Renderer::~Renderer()
{
  if ( sdl_renderer != nullptr )
  {
    SDL_DestroyRenderer(sdl_renderer);
    sdl_renderer = NULL;
  }
}
SDL_Renderer* Renderer::get_sdl_renderer()
{
  return sdl_renderer;
}
