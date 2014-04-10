#pragma once
#include <string>

/* Game on the waiting list. It is waiting for a second user.*/

struct GameInfo
{
  int game_id;
  std::string created_by;
  int creator_wins;
  int creator_losses;
  int num_players_in_game;
  int max_players_in_game;
};