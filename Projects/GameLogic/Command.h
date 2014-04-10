#pragma once
#include "Action.h"
#include "Directions.h"
#include "Point.h"

// A command is an instruction to be forwarded to a unit to perform.  Not
// all of its fields must be initialized for it to be used; in fact, for many
// operations the fields can be left in their default state.
struct Command
{
  unsigned long id;
  Action::Action_enum action;
  unsigned int count;
  Point pt;
  Direction dir;
  RelativeDirection rel_dir;
  int random;

  Command() : id(0), action(Action::Action_enum::MOVE_FORWARDS), count(0), pt(Point(0, 0)), dir(Direction::NORTH), rel_dir(RelativeDirection::RIGHT)
  {

  }
  JSONObject serialize() const;
  static Command unserialize(JSONObject object);
};