#pragma once
#include "Point.h"
#include "Directions.h"

class Position : Serializable
{
private:
  Point point;
  Direction facing_dir;
public:
  Position(Point ppoint, Direction pdir);
  Point get_point() const;
  Direction get_direction() const;
  virtual JSONObject serialize() const;
  static Position unserialize(JSONObject object);

};