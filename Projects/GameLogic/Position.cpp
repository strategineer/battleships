#include "Position.h"

Position::Position(Point ppoint, Direction pdir) : point(ppoint), facing_dir(pdir)
{}

Direction Position::get_direction() const
{
  return facing_dir;
}

Point Position::get_point() const
{
  return point;
}

JSONObject Position::serialize() const
{
  JSONObject pos_serialize = JSONObject::get_template("Class", "Position");
  pos_serialize.set_value("point", JSONElement_ptr(new JSONObject(point.serialize())));
  pos_serialize.set_value("facing_dir", JSONElement_ptr(new JSONString(std::to_string(facing_dir))));
  return pos_serialize;
}

Position Position::unserialize(JSONObject object)
{
  Point new_point = Point::unserialize(dynamic_cast<JSONObject&>( *object["point"] ));
  Direction new_facing_dir = static_cast<Direction>( atoi(JSONString::extract_string(object["facing_dir"]).c_str()) );
  return Position(new_point, new_facing_dir);

}