#include "Cannon.h"

int Cannon::default_cannon_damage = 1;
int Cannon::default_heavy_cannon_damage = 2;

Cannon::Cannon(int pdamage, int pright, int pleft, int pfront, int pback, CannonType ptype)
: Weapon(pdamage, pright, pleft, pfront, pback), type(ptype)
{};

Cannon::~Cannon()
{};

CannonShell Cannon::fire_cannon(Point fire_at) const
{
  return CannonShell(get_damage(), fire_at);
}

Cannon Cannon::get_default_cannon(int pright, int pleft, int pfront, int pback)
{
  return Cannon(default_cannon_damage, pright, pleft, pfront, pback, LIGHT);
}

Cannon Cannon::get_default_heavy_cannon(int pright, int pleft, int pfront, int pback)
{
  return Cannon(default_heavy_cannon_damage, pright, pleft, pfront, pback, HEAVY);
}

JSONObject Cannon::serialize() const
{
  JSONObject cannon_serialize = Weapon::serialize();
  cannon_serialize.set_value("name", JSONElement_ptr(new JSONString("Cannon")));
  cannon_serialize.set_value("type", JSONElement_ptr(new JSONString(std::to_string(type))));
  return cannon_serialize;
}

Cannon Cannon::unserialize(JSONObject object)
{
  int new_damage = atoi(JSONString::extract_string(object["damage"]).c_str());
  int new_right = atoi(JSONString::extract_string(object["right"]).c_str());
  int new_left = atoi(JSONString::extract_string(object["left"]).c_str());
  int new_front = atoi(JSONString::extract_string(object["front"]).c_str());
  int new_back = atoi(JSONString::extract_string(object["back"]).c_str());
  CannonType new_type = static_cast<CannonType>( atoi(JSONString::extract_string(object["type"]).c_str()) );
  return Cannon(new_damage, new_right, new_left, new_front, new_back, new_type);
}

