#pragma once
#include "SDL.h"
#include "Texture.h"
#include "EventHandler.h"
#include "Renderable.h"
#include "Font.h"
/* Simple clickeable button */
class Button
  : public EventHandler, public Renderable
{
public:
  /* Constructor does not set the Button's texture. */
  Button();
  /* Constructor sets the Button's texture. */
  Button(std::unique_ptr<Texture> txtr);
  ~Button();
  /* Set the Button's texture. */
  void set_texture(std::unique_ptr<Texture> txtr);
  /* Return true if the Button was clicked during previous handle_event call. */
  bool has_been_clicked();
  /* Allow the Button to be clicked and to change its appearance on hover and click. */
  void enable();
  /* Prevent the Button from being clicked and from changing its appearance on hover and click. */
  void disable();
  /* Set the position of the Button. */
  void set_position(int x, int y);
  /* Set the position of the Button by a certain factor of the screen's width and height. */
  void set_position_by_screen_factor(double width_factor, double height_factor);
private:
  enum ButtonState : unsigned char
  {
    Neither,
    Hover,
    Clicked,
    Disabled
  };
  virtual void do_handle_event(SDL_Event& e) override;
  virtual void do_render() override;
  ButtonState state;
  bool has_clicked, is_enabled;
  std::unique_ptr<Texture> texture;
  SDL_Point position;
};

