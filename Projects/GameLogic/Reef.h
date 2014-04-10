#pragma once
#include "ICollidable.h"
#include "Point.h"

class Reef :
  public ICollidable, Serializable
{
public:
  Reef(const Point & loc);
  ~Reef();
  virtual bool will_collide(const Point& pt) const override;
  virtual bool collide_at(const Point& pt, int damage) override;
  const Point & get_location() const;
  virtual JSONObject serialize() const;
  static Reef unserialize(JSONObject object);
private:
  Point location;
};

