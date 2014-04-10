#pragma once
#include <memory>
#include "GameState.h"

struct LoadedGame
{
  std::shared_ptr<GameState> loaded_gs;
  Player::Side current_turn_of;
  int new_game_id;
  bool success;
  std::string player_one;
  std::string player_two;
};
