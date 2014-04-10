#include "Explosive.h"

Explosive::Explosive(int pdamage) : damage(pdamage)
{

}

int Explosive::get_damage() const
{
  return damage;
}

Explosive::~Explosive()
{

}

JSONObject Explosive::serialize() const
{
  JSONObject exp_serialize = JSONObject::get_template("Class", "Explosive");
  exp_serialize.set_value("damage", JSONElement_ptr(new JSONString(std::to_string(damage))));
  return exp_serialize;
}

Explosive Explosive::unserialize(JSONObject object)
{
  int new_damage = atoi(JSONString::extract_string(object["damage"]).c_str());
  return Explosive(new_damage);
}