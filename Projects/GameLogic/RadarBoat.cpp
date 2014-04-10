#include "RadarBoat.h"

//TODO: remove movement actions from capabilities when deployed
RadarBoat::RadarBoat(const Position& initial_pos, int cell_health, int p_length,
  ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed, int p_rotation_speed,
  int p_center)
  : Boat(initial_pos, cell_health, p_length, p_collision_detector, pmine_handler, p_speed,
  p_rotation_speed, p_center, RADAR_BOAT)
{
  capabilities.push_back(Action::TOGGLE_SECONDARY_RADAR);
  is_deployed_flag = false;
}

RadarBoat::RadarBoat(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
  ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed,
  int p_rotation_speed, int p_center)
  : Boat(pid, maximum_health, pcells, p_location, p_collision_detector, pmine_handler, p_speed, p_rotation_speed, p_center, RADAR_BOAT)
{
  capabilities.push_back(Action::TOGGLE_SECONDARY_RADAR);
  is_deployed_flag = false;
}

RadarBoat::~RadarBoat()
{}

CommandResult RadarBoat::move_forwards(unsigned int distance)
{
  if ( is_deployed() )
  {
    CommandResult result(get_id());
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::CANNOT_MOVE;
    return result;
  }
  else
  {
    return Boat::move_forwards(distance);
  }
}

CommandResult RadarBoat::move_backwards(unsigned int distance)
{
  if ( is_deployed() )
  {
    CommandResult result(get_id());
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::CANNOT_MOVE;
    return result;
  }
  else
  {
    return Boat::move_backwards(distance);
  }
}

CommandResult RadarBoat::rotate(RelativeDirection dir, unsigned int rotation_units)
{
  if ( is_deployed() )
  {
    CommandResult result(get_id());
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::CANNOT_MOVE;
    return result;
  }
  else
  {
    return Boat::rotate(dir, rotation_units);
  }
}

CommandResult RadarBoat::shift(RelativeDirection dir)
{
  if ( is_deployed() )
  {
    CommandResult result(get_id());
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::CANNOT_MOVE;
    return result;
  }
  else
  {
    return Boat::shift(dir);
  }

}

CommandResult RadarBoat::perform_action(const Command& cmd)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::UNKNOWN_COMMAND;
  bool action_perfomed = false;
  switch ( cmd.action )
  {
  case Action::TOGGLE_SECONDARY_RADAR:
    toggle_deployed();
    result.result_type = CommandResult::SUCCESS;
    result.error_type = CommandResult::NO_ERROR;
    action_perfomed = true;
    break;
  }

  if ( action_perfomed )
  {
    return result;
  }

  return __super::perform_action(cmd);
}

void RadarBoat::set_deployed_radar_range(int l, int r, int f, int b)
{
  deployed_radar.set_range(l, r, f, b);
}
void RadarBoat::set_deployed_radar_level(Radar::Level lvl)
{
  deployed_radar.set_level(lvl);
}
void RadarBoat::toggle_deployed()
{
  is_deployed_flag = !is_deployed_flag;
}
bool RadarBoat::is_deployed()
{
  return is_deployed_flag;
}

std::shared_ptr<Unit> RadarBoat::s_construct_radar_boat(const Position& p, const RgbColor& col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<RadarBoat> b(new RadarBoat(p, 1, 3, collision_detector, pmine_handler, 3, 2, 1));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(2, 2, 6, -1);
  b->set_deployed_radar_level(Radar::Level::RADAR);
  b->set_deployed_radar_range(2, 2, 12, -1);
  b->capabilities.push_back(Action::Action_enum::FIRE_CANNON);
  b->capabilities.push_back(Action::Action_enum::TOGGLE_SECONDARY_RADAR);
  b->init_cannon(1, 1, 1, 1, Cannon::LIGHT);
  return std::dynamic_pointer_cast<Unit, RadarBoat>( b );
};

JSONObject RadarBoat::serialize() const
{
  JSONObject radarboat_serialize = Boat::serialize();
  radarboat_serialize.set_value("name", JSONElement_ptr(new JSONString("RadarBoat")));
  radarboat_serialize.set_value("deployed_radar", JSONElement_ptr(new JSONObject(deployed_radar.serialize())));
  radarboat_serialize.set_value("is_deployed_flag", JSONElement_ptr(new JSONString(std::to_string(is_deployed_flag))));

  return radarboat_serialize;
}

RadarBoat RadarBoat::unserialize(JSONObject object, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler)
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

  std::shared_ptr<Cannon> new_cannon;

  JSONElement_ptr new_cannon_json_ptr = object["cannon"];
  if ( JSONObject::is_json_object(new_cannon_json_ptr) )
  {
    new_cannon = std::shared_ptr<Cannon>(new Cannon(Cannon::unserialize(dynamic_cast<JSONObject&> ( *new_cannon_json_ptr ))));
  }

  int new_id = atoi(JSONString::extract_string(object["id"]).c_str());
  Radar new_radar = Radar::unserialize(dynamic_cast<JSONObject&> ( *object["radar"] ));
  Radar new_deployed_radar = Radar::unserialize(dynamic_cast<JSONObject&> ( *object["deployed_radar"] ));
  RgbColor new_color = RgbColor::unserialize(dynamic_cast<JSONObject&> ( *object["color"] ));
  int new_maximum_health = atoi(JSONString::extract_string(object["maximum_health"]).c_str());
  Position new_location = Position::unserialize(dynamic_cast<JSONObject&> ( *object["location"] ));
  UnitClass new_type = static_cast<UnitClass>( atoi(JSONString::extract_string(object["type"]).c_str()) );
  int new_length = atoi(JSONString::extract_string(object["length"]).c_str());
  int new_speed = atoi(JSONString::extract_string(object["speed"]).c_str());
  int new_rotation_speed = atoi(JSONString::extract_string(object["rotation_speed"]).c_str());
  int new_center = atoi(JSONString::extract_string(object["center"]).c_str());
  bool new_deployed_flag = atoi(JSONString::extract_string(object["is_deployed_flag"]).c_str()) == 1;

  //Create Unit object (can replace so that a special constructor or static method is used to create it)
  RadarBoat b(new_id, new_maximum_health, new_cells, new_location, p_collision_detector, pmine_handler, new_speed, new_rotation_speed, new_center);

  b.capabilities = new_capabilities;
  b.type = new_type;
  b.color = new_color;
  b.radar = new_radar;
  b.deployed_radar = new_deployed_radar;
  b.is_deployed_flag = new_deployed_flag;
  b.cannon = new_cannon;

  return b;
}

void RadarBoat::add_visibility(std::array<std::array<Radar::Level, 30>, 30> & board) const
{
  if ( is_deployed_flag )
  {
    deployed_radar.add_visibility(location, board);
  }
  else
  {
    Unit::add_visibility(board);
  }
}