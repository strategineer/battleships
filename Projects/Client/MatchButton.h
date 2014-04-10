#pragma once
#include "Button.h"
class MatchButton :
  public Button
{
public:
  MatchButton(int id, std::string txt);
  ~MatchButton();

  int get_game_id() const;
private:
  int game_id;
  std::string description;
};

