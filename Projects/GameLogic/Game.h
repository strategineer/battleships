#pragma once
#include "Player.h"
#include "ReefGenerator.h"
#include "Boat.h"
#include "GameState.h"

class Game
{
private:
  GameState game_state;
public:
  Game(Player one, Player two);
  void init_game();
  int save_game();

};