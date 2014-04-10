#include "Unit.h"

#include <limits>
#include "utilities.h"

unsigned long Unit::s_id = 0;

Unit::Unit(int health, int unit_length, const Position& p_location,
  ICollisionDetector *p_collision_detector) :
  maximum_health(health),
  location(p_location),
  length(unit_length),
  collision_detector(p_collision_detector),
  capabilities(),
  type(UNIT)
{
  id = s_id;
  if ( s_id != std::numeric_limits<unsigned long>::max() )
  {
    s_id++;
  }
  else
  {
    s_id = 0;
  }
  for ( int i = 0; i < unit_length; i++ )
  {
    cells.push_back(Cell(maximum_health));
  }
};

Unit::Unit(unsigned long pid, int pmaximum_health, std::vector<Cell> pcells, const Position& p_location,
  ICollisionDetector *p_collision_detector)
  :
  maximum_health(pmaximum_health),
  location(p_location),
  length(pcells.size()),
  collision_detector(p_collision_detector),
  capabilities(),
  type(UNIT),
  cells(pcells),
  id(pid)
{

}

void Unit::receive_damage(const Point& point, int amount)
{
  if ( contains_point(point) )
  {
    cells.at(Point::manhattan_distance(point, location.get_point())).subtract_health(amount);
  }
};

bool Unit::heal_damage(int amount)
{
  bool healed_damage = false;

  if ( amount <= 0 )
  {
    return false;
  }
  int heal_pos = -1;
  int i = 0;
  for ( auto it = cells.begin(); it != cells.end(); it++ )
  {
    if ( ( *it ).is_damaged() )
    {
      heal_pos = i;
    }
    i++;
  }
  if ( heal_pos >= 0 )
  {
    int to_heal = cells[heal_pos].get_damage();
    cells[heal_pos].add_health(( to_heal < amount ) ? to_heal : amount);
    healed_damage = true;
  }
  return healed_damage;
};

bool Unit::contains_point(const Point& point) const
{
  Point unit_point = location.get_point();
  switch ( location.get_direction() )
  {
  case Direction::NORTH:
    if ( point.get_x() == unit_point.get_x() )
    {
      if ( point.north_of(unit_point)
        && ( Point::manhattan_distance(unit_point, point) < length ) )
      {
        return true;
      }
    }
    break;
  case Direction::SOUTH:
    if ( point.get_x() == unit_point.get_x() )
    {
      if ( point.south_of(unit_point)
        && ( Point::manhattan_distance(unit_point, point) < length ) )
      {
        return true;
      }
    }
    break;
  case Direction::EAST:
    if ( point.get_y() == unit_point.get_y() )
    {
      if ( point.east_of(unit_point)
        && ( Point::manhattan_distance(unit_point, point) < length ) )
      {
        return true;
      }
    }
    break;
  case Direction::WEST:
    if ( point.get_y() == unit_point.get_y() )
    {
      if ( point.west_of(unit_point)
        && ( Point::manhattan_distance(unit_point, point) < length ) )
      {
        return true;
      }
    }
    break;
  }
  // Otherwise return false since the point is not within the unit. Note that
  // this assumes all units are one block wide.
  return false;
};

bool Unit::will_collide(const Point& pt) const
{
  return contains_point(pt);
}

bool Unit::collide_at(const Point& pt, int damage)
{
  receive_damage(pt, damage);
  return true;
}

std::vector<const Action::Action_enum>::const_iterator Unit::get_capabilities_head() const
{
  return capabilities.cbegin();
};

std::vector<const Action::Action_enum>::const_iterator Unit::get_capabilities_tail() const
{
  return capabilities.cend();
};

bool Unit::is_sunk() const
{
  for ( auto & c : cells )
  {
    if ( !c.is_destroyed() )
    {
      return false;
    }
  }
  return true;
}

CommandResult Unit::perform_action(const Command& cmd)
{
  // A Unit on its own cannot perform an action.  This is the base case.
  // We should also log the action type that failed to be performed, and its
  // arguments.
  CommandResult result(get_id());
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::UNKNOWN_COMMAND;
  return result;
}


Point Unit::get_bow() const
{
  int bow_x = location.get_point().get_x();
  int bow_y = location.get_point().get_y();

  switch ( location.get_direction() )
  {
  case NORTH:
    bow_y -= ( length - 1 );
    break;
  case SOUTH:
    bow_y += ( length - 1 );
    break;
  case EAST:
    bow_x += ( length - 1 );
    break;
  case WEST:
    bow_x -= ( length - 1 );
    break;
  }

  return Point(bow_x, bow_y);
};

const Position & Unit::get_position() const
{
  return location;
}

const int Unit::get_length() const
{
  return length;
}

const unsigned long Unit::get_id() const
{
  return id;
}

const RgbColor & Unit::get_color() const
{
  return color;
}

int Unit::get_maximum_health() const
{
  return maximum_health;
}

void Unit::set_color(const int r, const int g, const int b)
{
  color.red = r;
  color.green = g;
  color.blue = b;
}

void Unit::set_color(const RgbColor & col)
{
  set_color(col.red, col.green, col.blue);
}
void Unit::set_radar_range(int l, int r, int f, int b)
{
  radar.set_range(l, r, f, b);
}
void Unit::set_radar_level(Radar::Level lvl)
{
  radar.set_level(lvl);
}

void Unit::add_visibility(std::array<std::array<Radar::Level, 30>, 30> & board) const
{
  radar.add_visibility(location, board);
};

const Cell & Unit::get_cell_x_cells_away_from_back(int x) const
{
  return cells[x];
}

Unit::UnitClass Unit::get_unit_type() const
{
  // If this value is being returned, there is quite clearly an error.
  return Unit::UNIT;
}

Unit::~Unit()
{};

JSONObject Unit::serialize() const
{
  JSONObject unit_serialize = JSONObject::get_template("Class", "Unit");

  JSONArray cells_array;
  for ( auto it = cells.begin(); it != cells.end(); it++ )
  {
    cells_array.add(JSONElement_ptr(new JSONObject(( *it ).serialize())));
  }
  unit_serialize.set_value("cells", JSONElement_ptr(new JSONArray(cells_array)));

  JSONArray capabilities_array;
  for ( auto it = capabilities.begin(); it != capabilities.end(); it++ )
  {
    capabilities_array.add(JSONElement_ptr(new JSONString(std::to_string(*it))));
  }
  unit_serialize.set_value("capabilities", JSONElement_ptr(new JSONArray(capabilities_array)));

  unit_serialize.set_value("id", JSONElement_ptr(new JSONString(std::to_string(id))));
  unit_serialize.set_value("radar", JSONElement_ptr(new JSONObject(radar.serialize())));
  unit_serialize.set_value("color", JSONElement_ptr(new JSONObject(color.serialize())));
  unit_serialize.set_value("maximum_health", JSONElement_ptr(new JSONString(std::to_string(maximum_health))));
  unit_serialize.set_value("location", JSONElement_ptr(new JSONObject(location.serialize())));
  unit_serialize.set_value("type", JSONElement_ptr(new JSONString(std::to_string(type))));
  unit_serialize.set_value("length", JSONElement_ptr(new JSONString(std::to_string(length))));

  return unit_serialize;
}

Unit Unit::unserialize(JSONObject object, ICollisionDetector *p_collision_detector)
{
  std::vector<Cell> new_cells;
  std::vector<JSONElement_ptr> cells_json_element_list = dynamic_cast<JSONArray&> ( *object["cells"] ).get_elements();
  for ( auto it = cells_json_element_list.begin(); it != cells_json_element_list.end(); it++ )
  {
    new_cells.push_back(Cell::unserialize(dynamic_cast<JSONObject&>( **it )));
  }

  std::vector<const Action::Action_enum> new_capabilities;
  std::vector<JSONElement_ptr> capabilities_json_element_list = dynamic_cast<JSONArray&> ( *object["capabilities"] ).get_elements();
  for ( auto it = capabilities_json_element_list.begin(); it != capabilities_json_element_list.end(); it++ )
  {
    new_capabilities.push_back(static_cast<const Action::Action_enum>( atoi(JSONString::extract_string(*it).c_str()) ));
  }

  int new_id = atoi(JSONString::extract_string(object["id"]).c_str());
  Radar new_radar = Radar::unserialize(dynamic_cast<JSONObject&> ( *object["radar"] ));
  RgbColor new_color = RgbColor::unserialize(dynamic_cast<JSONObject&> ( *object["color"] ));
  int new_maximum_health = atoi(JSONString::extract_string(object["maximum_health"]).c_str());
  Position new_location = Position::unserialize(dynamic_cast<JSONObject&> ( *object["location"] ));
  UnitClass new_type = static_cast<UnitClass>( atoi(JSONString::extract_string(object["type"]).c_str()) );
  int new_length = atoi(JSONString::extract_string(object["length"]).c_str());

  //Create Unit object (can replace so that a special constructor or static method is used to create it)
  Unit u(new_id, new_maximum_health, new_cells, new_location, p_collision_detector);

  u.capabilities = new_capabilities;
  u.id = new_id;
  u.type = new_type;
  u.color = new_color;
  u.radar = new_radar;
  u.length = new_length;

  return u;
}

Direction Unit::get_direction() const
{
  return location.get_direction();
}

int Unit::get_size() const
{
  return cells.size();
}

bool Unit::is_unit(ICollidable* exp)
{
  if ( dynamic_cast<Unit*>( exp ) != NULL )
  {
    return true;
  }
  return false;
}

std::vector<Point> Unit::get_points() const
{
  return get_points_from(get_position(), get_size() - 1);
}

void Unit::destroy_unit()
{
  for ( Cell& cell : cells )
  {
    cell.subtract_health(maximum_health);
  }
}

void Unit::set_position(Position pos)
{
  location = pos;
}