#include "FiredTorpedo.h"
#include "utilities.h"

//Torpedo starts at the point after the launch position
// pdistance > 0
FiredTorpedo::FiredTorpedo(int pdamage, Position plaunch_position, int pdistance)
: Explosive(pdamage), current_position(plaunch_position), travel_distance(pdistance), distance_left(pdistance--)
{

}

FiredTorpedo::~FiredTorpedo()
{

}

int FiredTorpedo::get_distance_to_travel() const
{
  return travel_distance;
}

//Assumes distance is valid and won't go out of bounds
void FiredTorpedo::advance_torpedo()
{
  if ( distance_left != 0 )
  {
    current_position = Position(get_point_at(current_position, 1), current_position.get_direction());
    distance_left--;
  }
}

int FiredTorpedo::get_remaining_distance_to_travel() const
{
  return distance_left;
}

Position FiredTorpedo::get_current_position() const
{
  return current_position;
}

bool FiredTorpedo::can_advance() const
{
  return distance_left != 0;
}

bool FiredTorpedo::is_fired_torpedo(ICollidable* exp)
{
  if ( strcmp(typeid( *exp ).name(), "class FiredTorpedo") == 0 )
  {
    return true;
  }
  return false;
}
