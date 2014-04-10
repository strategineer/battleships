#include "CanonShell.h"

CannonShell::CannonShell(int pdamage, Point fire_at) : Explosive(pdamage), fired_at(fire_at)
{

}

CannonShell::~CannonShell()
{

}

Point CannonShell::get_fired_at_point()
{
  return fired_at;
}

bool CannonShell::is_cannon_shell(ICollidable* exp)
{
  if ( strcmp(typeid( *exp ).name(), "class CannonShell") == 0 )
  {
    return true;
  }
  return false;
}
