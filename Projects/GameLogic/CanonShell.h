#pragma once
#include "ICollidable.h"
#include "Explosive.h"

class CannonShell : public Explosive
{
public:
  CannonShell(int pdamage, Point fire_at);
  Point get_fired_at_point();
  static bool is_cannon_shell(ICollidable* exp);
  ~CannonShell();
private:
  Point fired_at;
};