#pragma once
#include <string>

enum Direction
{
  SOUTH, NORTH, WEST, EAST
};
enum RelativeDirection
{
  RIGHT, LEFT
};

int get_angle_from_direction(Direction dir);
Direction get_direction_after_turn(Direction facing_dir, RelativeDirection rel_dir);
Direction get_opposite_direction(Direction dir);