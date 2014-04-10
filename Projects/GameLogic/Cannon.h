#pragma once
#include "Weapon.h"
#include "CanonShell.h"

class Cannon : public Weapon
{
public:
  enum CannonType
  {
    HEAVY, LIGHT
  };
  Cannon(int pdamage, int pright, int pleft, int pfront, int pback, CannonType ptype);
  CannonShell fire_cannon(Point fire_at) const;
  virtual JSONObject serialize() const;
  static Cannon get_default_cannon(int pright, int pleft, int pfront, int pback);
  static Cannon get_default_heavy_cannon(int pright, int pleft, int pfront, int pback);
  static Cannon unserialize(JSONObject object);

  ~Cannon();
private:
  static int default_cannon_damage;
  static int default_heavy_cannon_damage;
  CannonType type;

};