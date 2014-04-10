#pragma once
#include <cstdint>
#include "Serializable.h"

class Cell : public Serializable
{
private:
  int max_health;
  int current_health;
public:
  Cell(int health);
  bool add_health(int amount);
  bool subtract_health(int amount);
  bool is_damaged() const;
  bool is_destroyed() const;
  int get_current_health() const;
  int get_max_health() const;
  int get_damage() const;
  virtual JSONObject serialize() const;

  static Cell unserialize(JSONObject object);
};
