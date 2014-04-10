#include "Button.h"

Button::Button() : texture(std::unique_ptr<Texture>(new Texture()))
{
  state = ButtonState::Neither;
  has_clicked = false;
  is_enabled = true;
  position.x = 0;
  position.y = 0;
}
Button::Button(std::unique_ptr<Texture> txtr) : texture(std::move(txtr))
{
  state = ButtonState::Neither;
  has_clicked = false;
  is_enabled = true;
  position.x = 0;
  position.y = 0;
}
Button::~Button() {}

void Button::set_texture(std::unique_ptr<Texture> txtr)
{
  texture = std::move(txtr);
}
void Button::enable()
{
  is_enabled = true;
  state = ButtonState::Neither;
  texture->set_color(255, 255, 255);
}
void Button::disable()
{
  is_enabled = false;
  state = ButtonState::Disabled;
  texture->set_color(25, 25, 25);
}
void Button::set_position(int x, int y)
{
  texture->set_position(x, y);
  position = texture->get_position();
}
void Button::set_position_by_screen_factor(double width_factor, double height_factor)
{
  texture->set_position_by_screen_factor(width_factor, height_factor);
  position = texture->get_position();
}
void Button::do_handle_event(SDL_Event& e)
{
  has_clicked = false;
  if ( !is_enabled ) { return; }
  //If mouse event happened
  if ( e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
  {
    //Get mouse position
    int x, y;
    SDL_GetMouseState(&x, &y);
    //Check if mouse is in button
    bool inside = true;
    //Mouse is left of the button
    if ( x < position.x ) { inside = false; }
    //Mouse is right of the button
    else if ( x > position.x + texture->get_width() ) { inside = false; }
    //Mouse above the button
    else if ( y < position.y ) { inside = false; }
    //Mouse below the button
    else if ( y > position.y + texture->get_height() ) { inside = false; }
    //Mouse is outside button
    if ( !inside )
    {
      if ( state != ButtonState::Neither )
      {
        state = ButtonState::Neither;
        texture->set_color(255, 255, 255);
      }
    }
    //Mouse is inside button
    else
    {
      //Set mouse over sprite
      switch ( e.type )
      {
      case SDL_MOUSEMOTION:
        if ( state != ButtonState::Hover )
        {
          state = ButtonState::Hover;
          texture->set_color(175, 175, 175);
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        if ( state != ButtonState::Clicked )
        {
          state = ButtonState::Clicked;
          texture->set_color(75, 75, 75);
          has_clicked = true;
        }
        break;
      case SDL_MOUSEBUTTONUP:
        break;
      }
    }
  }
}

void Button::do_render()
{
  texture->render();
}

bool Button::has_been_clicked()
{
  return has_clicked;
}