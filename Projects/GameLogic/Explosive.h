#pragma once
#include "ICollidable.h"

class Explosive : public ICollidable, Serializable
{
private:
  int damage;
public:
  Explosive(int pdamage);
  int get_damage() const;
  virtual JSONObject serialize() const;
  static Explosive unserialize(JSONObject object);
  ~Explosive();
};