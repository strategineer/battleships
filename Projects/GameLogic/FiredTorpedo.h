#pragma once
#include "ICollidable.h"
#include "Explosive.h"
#include "Position.h"

class FiredTorpedo : public Explosive
{
private:
  Position current_position;
  int distance_left;
  int travel_distance;
public:
  FiredTorpedo(int pdamage, Position launch_position, int pdistance);
  void advance_torpedo();
  Position get_current_position() const;
  int get_distance_to_travel() const;
  int get_remaining_distance_to_travel() const;
  bool can_advance() const;
  static bool is_fired_torpedo(ICollidable* exp);
  ~FiredTorpedo();
};