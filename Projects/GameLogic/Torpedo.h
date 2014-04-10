#pragma once
#include "Weapon.h"
#include "FiredTorpedo.h"

class Torpedo : public Weapon
{
public:
  Torpedo(int damage, int distance);
  FiredTorpedo fire_torpedo(Position launch_position, int travel_distance) const;
  int get_max_distance();
  static Torpedo get_default_torpedo();
  virtual JSONObject serialize() const;
  static Torpedo unserialize(JSONObject object);

private:
  static int default_damage;
  static int default_distance;
  int max_distance;
};
