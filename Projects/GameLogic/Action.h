#pragma once
#include <string>

struct Action_t
{

private:
  std::string name;

public:
  std::string get_name() const
  {
    return name;
  }

  Action_t(std::string act_name) :
    name(act_name)
  {};

};

class Action
{
public:
  enum Action_enum
  {
    KAMIKAZE_MOVE,
    EXPLODE,
    MOVE_FORWARDS,
    MOVE_BACKWARDS,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_BACK,
    SLIDE_LEFT,
    SLIDE_RIGHT,
    LAY_MINE,
    PICK_UP_MINE,
    FIRE_TORPEDO,
    FIRE_CANNON,
    TOGGLE_SECONDARY_RADAR,
    REPAIR,
    SWAP_BOATS
  };
  // I'm violating the style guide here. We can decide if the 's_' prefix
  // should be added to each of these.

  static const Action_t move_forwards;
  static const Action_t move_backwards;
  static const Action_t turn;
  static const Action_t slide;
  static const Action_t lay_mine;
  static const Action_t pick_up_mine;
  static const Action_t fire_torpedo;
  static const Action_t fire_cannon;
  static const Action_t toggle_secondary_radar;
  static const Action_t repair;
  static const Action_t kamikaze_move;
  static const Action_t explode;

};

