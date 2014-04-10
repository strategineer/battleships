#include "Cell.h"
#include <sstream>

Cell::Cell(int health) :
current_health(health),
max_health(health)
{}

bool Cell::add_health(int amount)
{
  if ( amount < 0 )
  {
    return false;
  }
  else if ( amount < ( max_health - current_health ) )
  {
    current_health += amount;
  }
  else
  {
    current_health = max_health;
  }
  return true;
};

bool Cell::is_damaged() const
{
  return ( current_health < max_health );
};

bool Cell::is_destroyed() const
{
  return current_health == 0;
}

bool Cell::subtract_health(int amount)
{
  if ( amount < 0 )
  {
    return false;
  }
  else if ( amount < current_health )
  {
    current_health -= amount;
  }
  else
  {
    current_health = 0;
  }
  return true;
};

int Cell::get_damage() const
{
  return ( max_health - current_health );
}

JSONObject Cell::serialize() const
{
  JSONObject cell_json = JSONObject::get_template("Class", "Cell");
  cell_json.set_value("max_health", JSONElement_ptr(new JSONString(std::to_string(max_health))));
  cell_json.set_value("current_health", JSONElement_ptr(new JSONString(std::to_string(current_health))));
  return cell_json;
}

Cell Cell::unserialize(JSONObject object)
{
  int new_max_health = atoi(JSONString::extract_string(object["max_health"]).c_str());
  int new_current_health = atoi(JSONString::extract_string(object["current_health"]).c_str());
  Cell c(new_max_health);
  c.current_health = new_current_health;
  return c;
}

int Cell::get_current_health() const
{
  return current_health;
}

int Cell::get_max_health() const
{
  return max_health;
}

