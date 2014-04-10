#include "Window.h"


Window::Window(SDL_Window* wind) : sdl_window(wind)
{
}
Window::~Window()
{
  if ( sdl_window != nullptr )
  {
    SDL_DestroyWindow(sdl_window);
    sdl_window = NULL;
  }
}
SDL_Window* Window::get_sdl_window()
{
  return sdl_window;
}
