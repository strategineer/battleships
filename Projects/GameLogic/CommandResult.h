#pragma once
#include "Point.h"
#include "ICollidable.h"
#include <memory>
#include "Action.h"

struct CommandResult
{
  enum ResultType
  {
    SUCCESS, ERROR
  };
  enum ResultInformation
  {
    NO_INFORMATION, HIT, MISS, COLLISION
  };
  enum ErrorType
  {
    NO_ERROR, UNKNOWN_COMMAND, NO_TORPEDO, NO_CANNON, NO_MINE_DROPPER, OUT_OF_RANGE, OUT_OF_BOUND, CANNOT_DROP_MINE, NO_MINE_TO_PICKUP, INVALID_ROTATE, CANNOT_MOVE,
    INVALID_OBJECT, NO_OBJECT, NO_MINES_LEFT, CANNOT_MOVE_AT_SPECIFIED_POINT, NO_PATH_EXISTS
  };
  unsigned long id;
  Action::Action_enum action;
  ResultType result_type;
  ResultInformation result_information;
  Point result_point;
  std::vector<std::pair<Point, std::shared_ptr<ICollidable>>> result_collidables;
  ErrorType error_type;
  unsigned int count;
  bool game_won;
  CommandResult(int pid);
  bool is_valid_command();
};