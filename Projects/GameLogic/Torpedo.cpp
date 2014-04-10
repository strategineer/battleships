#include "Torpedo.h"
#include <algorithm>

int Torpedo::default_damage = 2;
int Torpedo::default_distance = 10;

Torpedo::Torpedo(int damage, int pdistance) : Weapon(damage, 0, 0, pdistance, 0), max_distance(pdistance)
{

}

Torpedo Torpedo::get_default_torpedo()
{
  return Torpedo(default_damage, default_distance);
}

FiredTorpedo Torpedo::fire_torpedo(Position launch_position, int travel_distance) const
{
  int distance = std::min(travel_distance, max_distance);
  return FiredTorpedo(get_damage(), launch_position, distance);
}

JSONObject Torpedo::serialize() const
{
  JSONObject torpedo_serialize = Weapon::serialize();
  torpedo_serialize.set_value("name", JSONElement_ptr(new JSONString("Torpedo")));
  torpedo_serialize.set_value("max_distance", JSONElement_ptr(new JSONString(std::to_string(max_distance))));
  return torpedo_serialize;

}


Torpedo Torpedo::unserialize(JSONObject object)
{
  int new_damage = atoi(JSONString::extract_string(object["damage"]).c_str());
  int new_max_distance = atoi(JSONString::extract_string(object["max_distance"]).c_str());
  return Torpedo(new_damage, new_max_distance);
}

int Torpedo::get_max_distance()
{
  return max_distance;
}