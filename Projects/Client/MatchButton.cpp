#include "MatchButton.h"
#include "Fonts.h"
#include "Colors.h"


MatchButton::MatchButton(int id, std::string txt) : game_id(id), description(txt)
{
  set_texture(std::unique_ptr<Texture>(new Texture(description, Fonts::get_instance()->MESSAGE_FONT, Colors::GREEN)));
}

MatchButton::~MatchButton()
{}

int MatchButton::get_game_id() const
{
  return game_id;
}