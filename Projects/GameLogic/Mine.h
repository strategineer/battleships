#pragma once
#include "Explosive.h"
#include <algorithm>
#include "utilities.h"

class Mine : public Explosive
{
public:
  Mine(Point point_on_board, int pdamage);
  bool operator==( const Mine& other );
  bool operator!=( const Mine& other );
  const Point get_point() const;
  virtual JSONObject serialize() const;
  static Mine unserialize(JSONObject object);
  virtual bool will_collide(const Point& pt) const;
  virtual bool collide_at(const Point& pt, int damage);
  static bool is_mine(ICollidable* exp);
  ~Mine();
private:
  Point dropped_at;
  std::vector<Point> collision_zone;
};