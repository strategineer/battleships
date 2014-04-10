#pragma once
#include "SDL_ttf.h"
/* Wrapper class for a TTF_Font*. */
class Font
{
public:
  Font(TTF_Font* font);
  ~Font();
  TTF_Font* get_sdl_font();
private:
  TTF_Font* sdl_font;
};

