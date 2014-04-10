#include "Notification.h"
#include "Fonts.h"
#include "Colors.h"


Notification::Notification(std::string const msg, int delay) : Notification(msg, delay, Colors::WHITE)
{}

Notification::Notification(std::string const msg, int delay, const SDL_Color color)
{
  start_ticks = SDL_GetTicks();
  time_in_ms = delay;
  texture = std::unique_ptr<Texture>(new Texture(msg, Fonts::get_instance()->MESSAGE_FONT, color));
}

Notification::~Notification()
{}

std::unique_ptr<Texture> & Notification::get_texture()
{
  return texture;
}

bool Notification::has_expired() const
{
  return ( SDL_GetTicks() - start_ticks ) > time_in_ms;
}
void Notification::do_render()
{
  texture->render();
}
