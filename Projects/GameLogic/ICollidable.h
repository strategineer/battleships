#pragma once
#include "Point.h"

class ICollidable
{
public:
  enum ICollidableType
  {
    UNKNOWN,
    BOAT,
    BASE,
    MINE,
    REEF
  };
  virtual bool will_collide(const Point& pt) const;
  virtual bool collide_at(const Point& pt, int damage);
  static ICollidableType extract_type(std::shared_ptr<ICollidable> c);
};

