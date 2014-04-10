#include "Fonts.h"

std::unique_ptr<Fonts> Fonts::instance = nullptr;

std::unique_ptr<Fonts> const & Fonts::get_instance()
{
  return instance;
}
void Fonts::set_instance()
{
  instance = std::unique_ptr<Fonts>(new Fonts);
}
void Fonts::release_instance()
{
  if ( instance != nullptr )
  {
    instance.release();
  }
}
Fonts::Fonts()
{
  TITLE_FONT = std::unique_ptr<Font>(new Font(TTF_OpenFont("fonts/mypager.ttf", 90)));
  MENU_FONT = std::unique_ptr<Font>(new Font(TTF_OpenFont("fonts/mypager.ttf", 50)));
  MESSAGE_FONT = std::unique_ptr<Font>(new Font(TTF_OpenFont("fonts/xmasxpress.ttf", 30)));
}

Fonts::~Fonts()
{
}