#include "Weapon.h"
#include "utilities.h"
#include "Directions.h"

Weapon::Weapon(int pdamage, int pright, int pleft, int pfront, int pback)
: damage(pdamage), right(pright), left(pleft), front(pfront), back(pback)
{};

Weapon::~Weapon()
{};

int Weapon::get_damage() const
{
  return damage;
}

std::vector<Point> Weapon::get_weapon_range(Position position, int unit_size) const
{
  return get_points_in_range(position, unit_size, front, back, left, right);
}

JSONObject Weapon::serialize() const
{
  JSONObject weap_serialize = JSONObject::get_template("Class", "Weapon");
  weap_serialize.set_value("damage", JSONElement_ptr(new JSONString(std::to_string(damage))));
  weap_serialize.set_value("right", JSONElement_ptr(new JSONString(std::to_string(right))));
  weap_serialize.set_value("left", JSONElement_ptr(new JSONString(std::to_string(left))));
  weap_serialize.set_value("front", JSONElement_ptr(new JSONString(std::to_string(front))));
  weap_serialize.set_value("back", JSONElement_ptr(new JSONString(std::to_string(back))));
  return weap_serialize;
}

Weapon Weapon::unserialize(JSONObject object)
{
  int new_damage = atoi(JSONString::extract_string(object["damage"]).c_str());
  int new_right = atoi(JSONString::extract_string(object["right"]).c_str());
  int new_left = atoi(JSONString::extract_string(object["left"]).c_str());
  int new_front = atoi(JSONString::extract_string(object["front"]).c_str());
  int new_back = atoi(JSONString::extract_string(object["back"]).c_str());
  return Weapon(new_damage, new_right, new_left, new_front, new_back);
}

bool Weapon::is_in_range(Position unit_position, int unit_size, Point target) const
{
  std::vector<Point> range = get_weapon_range(unit_position, unit_size);
  if ( std::find(range.begin(), range.end(), target) != range.end() )
  {
    return true;
  }
  return false;
}