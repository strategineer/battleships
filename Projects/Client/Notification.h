#pragma once
#include "SDL.h"
#include "Renderable.h"
#include "Texture.h"
/* Class representing a brief message shown to the player for a certain amount of time. */
class Notification
  : public Renderable
{
public:
  /* Constructor taking a message to show and a time in ms to display the given message for. */
  Notification(std::string const msg, int delay);
  Notification::Notification(std::string const msg, int delay, const SDL_Color color);
  ~Notification();
  /* returns true if this's time_in_ms has passed. */
  bool has_expired() const;
  /* returns a reference to this's texture. */
  std::unique_ptr<Texture> &get_texture();
private:
  void do_render();
  std::unique_ptr<Texture> texture;
  /* Ticks when the message was created. */
  Uint32 start_ticks;
  /* Time in ms to display the message for. */
  int time_in_ms;
};

