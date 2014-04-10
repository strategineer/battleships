#include "CommandResult.h"

CommandResult::CommandResult(int pid)
: result_type(ERROR), result_collidables(std::vector<std::pair<Point, std::shared_ptr<ICollidable>>>()), error_type(UNKNOWN_COMMAND),
count(0), game_won(false), result_information(NO_INFORMATION), id(pid)
{}

bool CommandResult::is_valid_command()
{
  if ( result_type == CommandResult::ERROR )
  {
    return false;
  }
  return true;
}
