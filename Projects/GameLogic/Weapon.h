#pragma once
#include <vector>
#include "Point.h"
#include "Position.h"

class Weapon : public Serializable
{
private:
  int damage;
  int right, left, front, back;
public:
  Weapon(int pdamage, int pright, int pleft, int pfront, int pback);
  virtual ~Weapon();
  int get_damage() const;
  virtual std::vector<Point> get_weapon_range(Position position, int unit_size) const;
  virtual JSONObject serialize() const;
  static Weapon unserialize(JSONObject object);
  bool is_in_range(Position unit_position, int unit_size, Point target) const;
};