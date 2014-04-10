#include "Font.h"

Font::Font(TTF_Font* font) : sdl_font(font)
{}


Font::~Font()
{
  if ( sdl_font != nullptr )
  {
    TTF_CloseFont(sdl_font);
    sdl_font = nullptr;
  }
}

TTF_Font* Font::get_sdl_font()
{
  return sdl_font;
}
