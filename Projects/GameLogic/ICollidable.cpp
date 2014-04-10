#include "ICollidable.h"

bool ICollidable::collide_at(const Point& pt, int damage)
{
  return false;
};

bool ICollidable::will_collide(const Point& pt) const
{
  return false;
};

ICollidable::ICollidableType ICollidable::extract_type(std::shared_ptr<ICollidable> c)
{
  if ( strcmp(typeid( *c ).name(), "class Boat") == 0 || strcmp(typeid( *c ).name(), "class RadarBoat") == 0 )
  {
    return ICollidableType::BOAT;
  }
  if ( strcmp(typeid( *c ).name(), "class Base") == 0 )
  {
    return ICollidableType::BASE;
  }
  if ( strcmp(typeid( *c ).name(), "class Mine") == 0 )
  {
    return ICollidableType::MINE;
  }
  if ( strcmp(typeid( *c ).name(), "class Reef") == 0 )
  {
    return ICollidableType::REEF;
  }
  return ICollidableType::UNKNOWN;
}