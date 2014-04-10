#include "Reef.h"


Reef::Reef(const Point & loc) : location(loc)
{}


Reef::~Reef()
{}

bool Reef::collide_at(const Point& pt, int damage)
{
  return location == pt;
};

bool Reef::will_collide(const Point& pt) const
{
  return location == pt;;
};

const Point & Reef::get_location() const
{
  return location;
}

JSONObject Reef::serialize() const
{
  JSONObject reef_serialize = JSONObject::get_template("Class", "Reef");
  reef_serialize.set_value("location", JSONElement_ptr(new JSONObject(location.serialize())));
  return reef_serialize;
}

Reef Reef::unserialize(JSONObject object)
{
  Point new_location = Point::unserialize(dynamic_cast<JSONObject&> ( *object["location"] ));
  return Reef(new_location);
}
