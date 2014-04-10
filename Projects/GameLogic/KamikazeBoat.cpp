#include "KamikazeBoat.h"
#include "utilities.h"

bool path_exists(std::array<std::array<bool, 30>, 30>& move_range, std::array<std::array<bool, 30>, 30>& collision_map, std::vector<Point>& directions, Point p, Point& target);
bool move_to(std::array<std::array<bool, 30>, 30>& move_range, std::array<std::array<bool, 30>, 30>& collision_map, std::vector<Point>& directions, Point p, Point& target);

KamikazeBoat::KamikazeBoat(const Position& initial_pos, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed) :
kamikaze_damage(2),
Boat(initial_pos, 2, 1, p_collision_detector, pmine_handler, p_speed, 0, 0, BoatType::KAMIKAZE_BOAT)
{

}

KamikazeBoat::KamikazeBoat(unsigned long pid, std::vector<Cell> pcells, const Position& p_location,
  ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler) :
  kamikaze_damage(2),
  Boat(pid, 2, pcells, p_location, p_collision_detector, pmine_handler, 2, 0, 0, BoatType::KAMIKAZE_BOAT)
{

}

KamikazeBoat::~KamikazeBoat()
{

}

std::shared_ptr<Unit> KamikazeBoat::s_construct_kamikaze_boat(const Position & p, const RgbColor & col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<KamikazeBoat> b(new KamikazeBoat(p, collision_detector, pmine_handler, 2));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(2, 2, 2, 2);
  b->capabilities.clear();
  b->capabilities.push_back(Action::KAMIKAZE_MOVE);
  b->capabilities.push_back(Action::EXPLODE);
  return std::dynamic_pointer_cast<Unit, KamikazeBoat>( b );
}

CommandResult KamikazeBoat::move(Point target, bool kamikaze)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;
  result.result_information = CommandResult::NO_INFORMATION;
  result.result_point = target;
  bool explode_without_moving = ( target == location.get_point() && kamikaze );
  if ( is_in_range(target) || explode_without_moving )
  {
    auto collidable = collision_detector->collision_check(target);
    if ( collidable != nullptr && !explode_without_moving )
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::CANNOT_MOVE_AT_SPECIFIED_POINT;
    }
    else if ( !check_if_path_exists(target) && !explode_without_moving )
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::NO_PATH_EXISTS;
    }
    else
    {
      location = Position(target, EAST);
      if ( kamikaze )
      {
        destroy_unit();
        Point start_point = target.get_west_by(1).get_north_by(1);
        std::vector<Point> collision_points;
        for ( int i = 0; i < 3; i++ )
        {
          Point new_start_point = start_point.get_east_by(i);
          for ( int j = 0; j < 3; j++ )
          {
            Point some_point = new_start_point.get_south_by(j);
            if ( !is_out_of_bounds(some_point) && location.get_point() != some_point )
            {
              collision_points.push_back(some_point);
            }
          }
        }
        for ( auto it = collision_points.begin(); it != collision_points.end(); it++ )
        {
          auto collidable = collision_detector->collision_check(*it);
          if ( collidable != nullptr )
          {
            if ( Mine::is_mine(collidable.get()) )
            {
              mine_handler->remove_mine(*it);
            }
            else if ( Unit::is_unit(collidable.get()) )
            {
              dynamic_cast<Unit &>( *collidable ).receive_damage(*it, 2);
            }
            result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(*it, collidable));
          }
        }
      }
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::OUT_OF_RANGE;
  }
  return result;
}

std::array<std::array<bool, 30>, 30> KamikazeBoat::get_move_range() const
{
  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> range;
  range.fill(row);
  int distance = 2 * get_speed() + 1;
  Point start_point = location.get_point().get_west_by(get_speed()).get_north_by(get_speed());
  for ( int i = 0; i < distance; i++ )
  {
    Point new_start_point = start_point.get_east_by(i);
    for ( int j = 0; j < distance; j++ )
    {
      Point some_point = new_start_point.get_south_by(j);
      if ( !is_out_of_bounds(some_point) && location.get_point() != some_point )
      {
        range[some_point.get_x()][some_point.get_y()] = true;
      }
    }
  }
  return range;
}

bool KamikazeBoat::is_in_range(Point target) const
{
  return get_move_range()[target.get_x()][target.get_y()];
}

JSONObject KamikazeBoat::serialize() const
{
  JSONObject kamikaze_serialize = Boat::serialize();
  kamikaze_serialize.set_value("name", JSONElement_ptr(new JSONString("KamikazeBoat")));
  kamikaze_serialize.set_value("kamikaze_damage", JSONElement_ptr(new JSONString(std::to_string(kamikaze_damage))));
  return kamikaze_serialize;
}

KamikazeBoat KamikazeBoat::unserialize(JSONObject object, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler)
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
  int new_kamikaze_damage = atoi(JSONString::extract_string(object["kamikaze_damage"]).c_str());
  Radar new_radar = Radar::unserialize(dynamic_cast<JSONObject&> ( *object["radar"] ));
  RgbColor new_color = RgbColor::unserialize(dynamic_cast<JSONObject&> ( *object["color"] ));
  Position new_location = Position::unserialize(dynamic_cast<JSONObject&> ( *object["location"] ));

  //Create Unit object (can replace so that a special constructor or static method is used to create it)
  KamikazeBoat kb(new_id, new_cells, new_location, p_collision_detector, pmine_handler);

  kb.capabilities = new_capabilities;
  kb.type = UnitClass::BOAT;
  kb.color = new_color;
  kb.radar = new_radar;
  kb.kamikaze_damage = new_kamikaze_damage;

  return kb;
}

CommandResult KamikazeBoat::perform_action(const Command& cmd)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::UNKNOWN_COMMAND;
  bool action_performed = false;
  switch ( cmd.action )
  {
  case Action::KAMIKAZE_MOVE:
    result = move(cmd.pt, false);
    action_performed = true;
    break;
  case Action::EXPLODE:
    result = move(cmd.pt, true);
    action_performed = true;
    break;
  }
  if ( action_performed )
  {
    return result;
  }
  return __super::perform_action(cmd);
}

bool KamikazeBoat::check_if_path_exists(Point target)
{
  auto move_range = get_move_range();
  move_range[location.get_point().get_x()][location.get_point().get_y()] = true;
  std::array<bool, 30> row;
  std::array<std::array<bool, 30>, 30> collision_map;
  collision_map.fill(row);
  for ( int i = 0; i < 30; i++ )
  {
    for ( int j = 0; j < 30; j++ )
    {
      if ( collision_detector->collision_check(Point(i,j)) != nullptr )
      {
        collision_map[i][j] = true;
      }
      else
      {
        collision_map[i][j] = false;
      }
    }
  }
  if ( collision_map[target.get_x()][target.get_y()] )
  {
    return false;
  }
  std::vector<Point> directions;
  return path_exists(move_range, collision_map, directions, location.get_point(), target);
}

bool path_exists(std::array<std::array<bool, 30>, 30>& move_range, std::array<std::array<bool, 30>, 30>& collision_map, std::vector<Point>& directions, Point p, Point& target)
{
  int x = p.get_x();
  int y = p.get_y();
  if ( !is_out_of_bounds(p) && move_range[x][y] )
  {
    if ( move_to(move_range, collision_map, directions, Point(x - 1, y), target) )
    {
      return true;
    }
    if ( move_to(move_range, collision_map, directions, Point(x, y - 1), target) )
    {
      return true;
    }
    if ( move_to(move_range, collision_map, directions, Point(x + 1, y), target) )
    {
      return true;
    }
    if ( move_to(move_range, collision_map, directions, Point(x, y + 1), target) )
    {
      return true;
    }
  }
  return false;
}

bool move_to(std::array<std::array<bool, 30>, 30>& move_range, std::array<std::array<bool, 30>, 30>& collision_map, std::vector<Point>& directions, Point p, Point& target)
{
  int x = p.get_x();
  int y = p.get_y();
  if ( !is_out_of_bounds(p) && move_range[x][y] && std::find(directions.begin(), directions.end(), p) == directions.end() )
  {
    if ( !collision_map[x][y] )
    {
      if ( p == target )
      {
        return true;
      }
      directions.push_back(p);
      if ( path_exists(move_range, collision_map, directions, p, target) )
      {
        return true;
      }
      else
      {
        directions.pop_back();
      }
    }
  }
  return false;
}
