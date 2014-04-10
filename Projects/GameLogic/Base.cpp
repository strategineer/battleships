#include "Base.h"
#include "Directions.h"
#include "utilities.h"

// Note that for each player this should be modified to refer to their
// particular location
Base::Base() :
Unit(2, 10, Position(Point(1, 1), Direction::NORTH), nullptr)
{
  capabilities.push_back(Action::REPAIR);
  radar.set_level(Radar::Level::RADAR);
  radar.set_range(1, 1, 10, 1);
};

Base::Base(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
  ICollisionDetector *p_collision_detector)
  : Unit(pid, maximum_health, pcells, p_location, p_collision_detector)
{

}

std::array<std::array<bool, 30>, 30> Base::get_repair_range() const
{
  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> matrix_range;
  matrix_range.fill(row);

  std::vector<Point> vector_range;
  vector_range.push_back(get_bow().get_point_in_direction(location.get_direction(), 1));
  vector_range.push_back(location.get_point().get_point_in_direction(get_opposite_direction(location.get_direction()), 1));
  std::vector<Point> base_points = get_points();
  for ( auto it = base_points.begin(); it != base_points.end(); it++ )
  {
    vector_range.push_back(( *it ).get_point_in_direction(get_direction_after_turn(location.get_direction(), RelativeDirection::RIGHT), 1));
    vector_range.push_back(( *it ).get_point_in_direction(get_direction_after_turn(location.get_direction(), RelativeDirection::LEFT), 1));
  }
  for ( auto it = vector_range.begin(); it != vector_range.end(); it++ )
  {
    if ( !is_out_of_bounds(*it) )
    {
      matrix_range[( *it ).get_x()][( *it ).get_y()] = true;
    }
  }
  return matrix_range;
}

CommandResult Base::repair_boat(Point target)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.result_information = CommandResult::NO_INFORMATION;
  result.error_type = CommandResult::NO_ERROR;
  result.result_point = target;
  bool in_range = false;
  std::vector<Point> range_points = get_points_in_range(location, get_length(), 1, 1, 1, 1);
  for ( auto it = range_points.begin(); it != range_points.end(); it++ )
  {
    if ( target == ( *it ) )
    {
      in_range = true;
    }
  }
  if ( !in_range )
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::OUT_OF_RANGE;
    return result;
  }
  std::shared_ptr<ICollidable> other = collision_detector->collision_check(target);
  if ( other != nullptr )
  {
    result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(target, other));
    if ( ICollidable::extract_type(other) == ICollidableType::BOAT )
    {
      dynamic_cast<Unit&>( *other ).heal_damage(2);
      result.count = 2;
    }
    else
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::INVALID_OBJECT;
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::NO_OBJECT;
  }
  return result;
}

Base::Base(Position initial_position, int health, int length,
  ICollisionDetector *p_collision_detector) :
  Unit(health, length, initial_position, p_collision_detector)
{
  type = Unit::BASE;
  capabilities.push_back(Action::REPAIR);
  radar.set_level(Radar::Level::RADAR);
  radar.set_range(1, 1, 10, 1);
};

CommandResult Base::perform_action(const Command& cmd)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::UNKNOWN_COMMAND;
  bool action_performed = false;
  switch ( cmd.action )
  {
  case Action::REPAIR:
    // Use the Point provided to determine if a ship can be repaired, and
    // effect the repair.
    //TODO: check if the point collides with a docked boat if yes then repair it if its damaged
    result = repair_boat(cmd.pt);
    action_performed = true;
    break;
  }

  if ( action_performed )
  {
    return result;
  }

  return __super::perform_action(cmd);
}

Unit::UnitClass Base::get_unit_type() const
{
  return Unit::BASE;
};

std::shared_ptr<Unit> Base::s_construct_base(const Position& p, const RgbColor& col,
  ICollisionDetector *collision_detector)
{
  std::shared_ptr<Base> b(new Base(p, 2, 10, collision_detector));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(1, 1, 10, 1);
  b->capabilities.push_back(Action::Action_enum::REPAIR);
  return std::dynamic_pointer_cast<Unit, Base>( b );
}

JSONObject Base::serialize() const
{
  JSONObject base_serialize = Unit::serialize();
  base_serialize.set_value("name", JSONElement_ptr(new JSONString("Base")));
  return base_serialize;
}

Base Base::unserialize(JSONObject object, ICollisionDetector *p_collision_detector)
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
  Base b(new_id, new_maximum_health, new_cells, new_location, p_collision_detector);

  b.capabilities = new_capabilities;
  b.type = new_type;
  b.color = new_color;
  b.radar = new_radar;

  return b;
}
