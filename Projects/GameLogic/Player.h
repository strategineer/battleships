#pragma once
#include "Boat.h"
#include "Base.h"

class Player
{
public:
  enum Side
  {
    PLAYER_ONE, PLAYER_TWO, SPECTATOR
  };
private:
  std::vector<Boat> boats;
  Base base;
};