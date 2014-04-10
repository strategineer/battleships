#pragma once
#include "SDL_ttf.h"
#include <memory>
#include "Font.h"
/* 
  Singleton class responsible for storing, providing and releasing Font(s) 
  to be used for displaying text.
  */
class Fonts
{
public:
  ~Fonts();
  std::unique_ptr<Font> TITLE_FONT, MENU_FONT, MESSAGE_FONT;
  static std::unique_ptr<Fonts> const & get_instance();
  static void set_instance();
  /* 
    Releases/destroys the fonts.
    NB: must be called before SDL is shutdown/quit.
    */
  static void release_instance();
private:
  Fonts();
  static std::unique_ptr<Fonts> instance;
};

