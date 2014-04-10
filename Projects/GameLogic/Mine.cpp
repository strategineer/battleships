#include "Mine.h"


Mine::Mine(Point point_on_board, int pdamage)
: Explosive(pdamage), dropped_at(point_on_board)
{
  auto add_point = [](Mine *m, Point p) {if (!is_out_of_bounds(p)) { m->collision_zone.emplace_back(std::move(p)); }; };
  Point southly = point_on_board.get_south_by(1);
  add_point(this, southly);
  Point northly = point_on_board.get_north_by(1);
  add_point(this, northly);
  Point eastly = point_on_board.get_east_by(1);
  add_point(this, eastly);
  Point westly = point_on_board.get_west_by(1);
  add_point(this, westly);
  add_point(this, Point(dropped_at));
};


Mine::~Mine()
{

};

JSONObject Mine::serialize() const
{
  JSONObject mine_serialize = Explosive::serialize();
  mine_serialize.set_value("name", JSONElement_ptr(new JSONString("Mine")));
  mine_serialize.set_value("dropped_at", JSONElement_ptr(new JSONObject(dropped_at.serialize())));
  return mine_serialize;
}

Mine Mine::unserialize(JSONObject object)
{
  int new_damage = atoi(dynamic_cast<JSONString&> ( *object["damage"] ).get_string().c_str());
  Point new_dropped_at = Point::unserialize(dynamic_cast<JSONObject&> ( *object["dropped_at"] ));
  return Mine(new_dropped_at, new_damage);
}

const Point Mine::get_point() const
{
  return dropped_at;
}

bool Mine::will_collide(const Point& pt) const
{
  if (std::find(collision_zone.begin(), collision_zone.end(), pt) != collision_zone.end())
  {
    return true;
  }
  return false;
}

bool Mine::collide_at(const Point& pt, int damage)
{
  if (std::find(collision_zone.begin(), collision_zone.end(), pt) != collision_zone.end())
  {
    return true;
  }
  return false;
}

bool Mine::operator==( const Mine& other )
{
  return other.get_damage() == get_damage()
    && other.get_point() == get_point();
}

bool Mine::operator!=( const Mine& other )
{
  return !( *this == other );
}

bool Mine::is_mine(ICollidable* exp)
{
  if ( strcmp(typeid( *exp ).name(), "class Mine") == 0 )
  {
    return true;
  }
  return false;
}
