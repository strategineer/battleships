#include "Directions.h"

//Converts a direction to an angle (angle starts on the x axis of the positive x and positive y quadrant)
int get_angle_from_direction(Direction dir)
{
  int angle = 0;
  if ( dir == EAST )
  {
    angle = 0;
  }
  else if ( dir == NORTH )
  {
    angle = 90;
  }
  else if ( dir == WEST )
  {
    angle = 180;
  }
  else if ( dir == SOUTH )
  {
    angle = 270;
  }
  return angle;
};

//Get new facing direction after turning RIGHT or LEFT
Direction get_direction_after_turn(Direction facing_dir, RelativeDirection rel_dir)
{
  Direction new_facing_direction = facing_dir;
  if ( rel_dir == RIGHT )
  {
    if ( facing_dir == NORTH )
    {
      new_facing_direction = Direction::EAST;
    }
    else if ( facing_dir == EAST )
    {
      new_facing_direction = Direction::SOUTH;
    }
    else if ( facing_dir == SOUTH )
    {
      new_facing_direction = Direction::WEST;
    }
    else if ( facing_dir == WEST )
    {
      new_facing_direction = Direction::NORTH;
    }
  }
  else
  {
    if ( facing_dir == NORTH )
    {
      new_facing_direction = Direction::WEST;
    }
    else if ( facing_dir == WEST )
    {
      new_facing_direction = Direction::SOUTH;
    }
    else if ( facing_dir == SOUTH )
    {
      new_facing_direction = Direction::EAST;
    }
    else if ( facing_dir == EAST )
    {
      new_facing_direction = Direction::NORTH;
    }
  }
  return new_facing_direction;
};

//Get the opposite direction of which this element is facing
Direction get_opposite_direction(Direction dir)
{
  Direction opposite_dir = dir;

  if ( dir == NORTH )
  {
    opposite_dir = Direction::SOUTH;
  }
  else if ( dir == EAST )
  {
    opposite_dir = Direction::WEST;
  }
  else if ( dir == SOUTH )
  {
    opposite_dir = Direction::NORTH;
  }
  else if ( dir == WEST )
  {
    opposite_dir = Direction::EAST;
  }
  return opposite_dir;
};

std::string get_direction_as_string(Direction dir)
{
  std::string s_dir("");
  if ( dir == NORTH )
  {
    s_dir = "NORTH";
  }
  else if ( dir == EAST )
  {
    s_dir = "EAST";
  }
  else if ( dir == SOUTH )
  {
    s_dir = "SOUTH";
  }
  else if ( dir == WEST )
  {
    s_dir = "WEST";
  }
  return s_dir;
}

std::string get_relative_direction_as_string(RelativeDirection rel_dir)
{
  std::string s_rel_dir("");
  if ( rel_dir == RIGHT )
  {
    s_rel_dir = "RIGHT";
  }
  else if ( rel_dir == LEFT )
  {
    s_rel_dir = "LEFT";
  }
  return s_rel_dir;
}