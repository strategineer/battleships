#include "Boat.h"
#include "Directions.h"
#include "utilities.h"

Boat::Boat(const Position& initial_pos, int health, int p_length,
  ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler,
  int p_speed, int p_rotation_speed, int p_center, BoatType ptype) :
  Unit(health, p_length, initial_pos, p_collision_detector),
  speed(p_speed),
  rotation_speed(p_rotation_speed),
  center(p_center),
  mine_handler(pmine_handler),
  mine_dropper(nullptr),
  torpedo(nullptr),
  cannon(nullptr),
  boat_type(ptype)
{
  capabilities.push_back(Action::MOVE_BACKWARDS);
  capabilities.push_back(Action::MOVE_FORWARDS);
  capabilities.push_back(Action::SLIDE_LEFT);
  capabilities.push_back(Action::SLIDE_RIGHT);
  capabilities.push_back(Action::TURN_LEFT);
  capabilities.push_back(Action::TURN_RIGHT);
  if ( rotation_speed == 2 )
  {
    capabilities.push_back(Action::TURN_BACK);
  }

};

Boat::Boat(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
  ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed,
  int p_rotation_speed, int p_center, BoatType ptype)
  : Unit(pid, maximum_health, pcells, p_location, p_collision_detector),
  speed(p_speed),
  rotation_speed(p_rotation_speed),
  center(p_center),
  mine_handler(pmine_handler),
  mine_dropper(nullptr),
  torpedo(nullptr),
  cannon(nullptr),
  boat_type(ptype)
{

}

int Boat::get_speed() const
{
  int num_cells_damaged = 0;
  for ( int i = 0; i < get_size(); i++ )
  {
    if ( get_cell_x_cells_away_from_back(i).is_destroyed() )
    {
      num_cells_damaged++;
    }
  }
  double percent_speed_left = (double)( get_size() - num_cells_damaged ) / get_size();
  return (int)( speed * percent_speed_left );
}

CommandResult Boat::move_forwards(Point target)
{
  bool is_in_range = false;
  for ( int i = 0; i < get_speed(); i++ )
  {
    if ( target == get_bow().get_point_in_direction(get_direction(), i + 1) )
    {
      is_in_range = true;
      break;
    }
  }
  if ( is_in_range )
  {
    return move_forwards(Point::manhattan_distance(get_bow(), target));
  }
  CommandResult result(get_id());
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::OUT_OF_RANGE;
  return result;
}

CommandResult Boat::move_forwards(unsigned int distance)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;

  unsigned int remaining_distance = ( distance < get_speed() ) ? distance : get_speed();
  // Move forwards until obstacle is encountered.
  unsigned int move_distance = 0;
  while ( remaining_distance > 0 )
  {
    // Create the point being moved into.
    Point current_bow = get_bow();
    Point next_bow;
    Point next_stern;
    switch ( location.get_direction() )
    {
    case NORTH:
      next_bow = current_bow.get_north_by(1);
      next_stern = location.get_point().get_north_by(1);
      break;
    case SOUTH:
      next_bow = current_bow.get_south_by(1);
      next_stern = location.get_point().get_south_by(1);
      break;
    case EAST:
      next_bow = current_bow.get_east_by(1);
      next_stern = location.get_point().get_east_by(1);
      break;
    case WEST:
      next_bow = current_bow.get_west_by(1);
      next_stern = location.get_point().get_west_by(1);
      break;
    }

    // Check for point being in the map.
    // Check for collision with next unit
    std::shared_ptr<ICollidable> other = nullptr;
    other = collision_detector->collision_check(next_bow);
    // Replace this with call to collision_check()
    // If collision, effect collision on both objects. Break.
    if ( other != nullptr )
    {
      handle_boat_collision(current_bow, next_bow, other, result);
      if ( result.result_information == CommandResult::COLLISION )
      {
        result.count = move_distance;
        break; // DO NOT MOVE FORWARDS
      }
    }
    // Else move forwards.
    // change position forward by one.
    location = Position(next_stern, location.get_direction());
    remaining_distance--;
    move_distance++;
  }
  return result;
};

CommandResult Boat::move_backwards(unsigned int distance)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;
  result.id = get_id();

  // What is the fastest reverse motion?
  unsigned int remaining_distance = 1; // I believe this is the reversing distance.
  unsigned int move_distance = 0;
  while ( remaining_distance > 0 )
  {
    Point current_stern = location.get_point();
    Point next_stern;
    switch ( location.get_direction() )
    {
    case NORTH:
      next_stern = current_stern.get_south_by(1);
      break;
    case SOUTH:
      next_stern = current_stern.get_north_by(1);
      break;
    case EAST:
      next_stern = current_stern.get_west_by(1);
      break;
    case WEST:
      next_stern = current_stern.get_east_by(1);
      break;
    }
    // Check for point being in the map.
    // Check for collision with anything behind the boat.
    std::shared_ptr<ICollidable> other = collision_detector->collision_check(next_stern);
    // If collision, effect collision on both objects. Break.
    if ( other != nullptr )
    {
      handle_boat_collision(current_stern, next_stern, other, result);
      if ( result.result_information == CommandResult::COLLISION )
      {
        result.count = move_distance;
        break; // Do not move back!
      }
    }
    // Else move backwards.
    location = Position(next_stern, location.get_direction());
    remaining_distance--;
    move_distance++;
  }
  return result;
};

CommandResult Boat::shift(RelativeDirection dir)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;

  bool move_succeeded = true;
  std::vector<std::pair<Point, Point>> colliding_points;
  Direction shift_direction = get_direction_after_turn(location.get_direction(), dir);
  Point iter_point = location.get_point();
  colliding_points.push_back(std::pair<Point, Point>(location.get_point(), iter_point.get_point_in_direction(shift_direction, 1)));
  // Collect all points where a collision might take place.
  for ( int i = length - 1; i >= 0; i-- )
  {
    iter_point = iter_point.get_point_in_direction(location.get_direction(), 1);
    colliding_points.push_back(std::pair<Point, Point>(iter_point, iter_point.get_point_in_direction(shift_direction, 1)));
  }
  // Test each point for collidability.

  for ( std::pair<Point, Point> p : colliding_points )
  {
    std::shared_ptr<ICollidable> other = collision_detector->collision_check(p.second);
    if ( other != nullptr )
    {
      // Collide.
      handle_boat_collision(p.first, p.second, other, result);
      if (result.result_type == CommandResult::ERROR)
      {
        return result;
      }
      if ( result.result_information == CommandResult::COLLISION || result.result_type == CommandResult::ERROR )
      {
        move_succeeded = false;
        result.count = 1;
      }
    }
  }

  if ( move_succeeded )
  {
    location = Position(location.get_point().get_point_in_direction(shift_direction, 1), location.get_direction());
  }

  return result;
};

CommandResult Boat::rotate(RelativeDirection dir, unsigned int rotation_units)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.result_information = CommandResult::NO_INFORMATION;
  result.error_type = CommandResult::NO_ERROR;
  Position last_pos = location;

  bool collided = false;
  if ( ( rotation_units == 0 ) || ( rotation_units > rotation_speed ) )
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::INVALID_ROTATE;
    return result;
  }
  while ( rotation_units > 0 )
  {
    std::vector<std::vector<Point>> stern_collision_points = std::vector<std::vector<Point>>();
    std::vector<std::vector<Point>> bow_collision_points = std::vector<std::vector<Point>>();

    int center_to_bow = ( length - center ) - 1;
    int center_to_stern = center;

    Direction turning_orientation = get_direction_after_turn(location.get_direction(), dir);
    Point rotation_point = get_rotation_point();
    // Build from this point using the turning direction and the current boat direction.
    // Build the set of points to iterate over
    // Points forward of the centre of rotation
    // Starts from point 1 in front of the centre of rotation and 1 in the relative direction of turning
    // Build a vector forward from that point to the bow of the ship
    // Repeat with same vector on next row, for one fewer points
    // Repeat until all rows are exhausted (there are zero points to iterate over)

    for ( int i = 1; i <= center_to_bow; i++ )
    {
      Point starting_point = rotation_point.get_point_in_direction(turning_orientation, i);
      starting_point = starting_point.get_point_in_direction(location.get_direction(), center_to_bow - i + 1);
      std::vector<Point> possible_collisions;
      int x = 0;
      int y = 0;

      for ( int counter = center_to_bow; counter > 0; counter--, y++, x++ )
      {
        Point possible_collision_point = starting_point.get_point_in_direction(get_opposite_direction(location.get_direction()), std::min(center_to_bow - i, y));
        possible_collision_point = possible_collision_point.get_point_in_direction(get_opposite_direction(turning_orientation), std::min(i - 1, x));
        if ( is_out_of_bounds(possible_collision_point) )
        {
          result.result_type = CommandResult::ERROR;
          result.error_type = CommandResult::OUT_OF_BOUND;
          //reset initial position (should we remove this. For example back of boat is out of bounds, a 180 degree turn will be cancelled or will do only 90 degree turn????)
          location = last_pos;
          return result;
        }
        //ensure uniqueness in possible collisions
        if ( std::find(possible_collisions.begin(), possible_collisions.end(), possible_collision_point) == possible_collisions.end() )
        {
          possible_collisions.push_back(possible_collision_point);
        }
      }
      bow_collision_points.push_back(possible_collisions);
    }

    for ( int i = 1; i <= center_to_stern; i++ )
    {
      Point starting_point = rotation_point.get_point_in_direction(get_opposite_direction(turning_orientation), i);
      starting_point = starting_point.get_point_in_direction(get_opposite_direction(location.get_direction()), center_to_stern - i + 1);
      std::vector<Point> possible_collisions;
      int x = 0;
      int y = 0;

      for ( int counter = center_to_stern; counter > 0; counter--, y++, x++ )
      {
        Point possible_collision_point = starting_point.get_point_in_direction(location.get_direction(), std::min(center_to_stern - i, y));
        possible_collision_point = possible_collision_point.get_point_in_direction(turning_orientation, std::min(i - 1, x));
        if ( is_out_of_bounds(possible_collision_point) )
        {
          result.result_type = CommandResult::ERROR;
          result.error_type = CommandResult::OUT_OF_BOUND;
          //reset initial position
          location = last_pos;
          return result;
        }
        //ensure uniqueness in possible collisions
        if ( std::find(possible_collisions.begin(), possible_collisions.end(), possible_collision_point) == possible_collisions.end() )
        {
          possible_collisions.push_back(possible_collision_point);
        }
      }
      stern_collision_points.push_back(possible_collisions);
    }

    for ( int i = 0; i < stern_collision_points.size() || i < bow_collision_points.size(); i++ )
    {
      if ( i < stern_collision_points.size() )
      {
        for ( int y = 0; y < stern_collision_points[i].size(); y++ )
        {
          Point boat_collision_point = location.get_point().get_point_in_direction(location.get_direction(), y);
          std::shared_ptr<ICollidable> other = collision_detector->collision_check(stern_collision_points[i][y]);
          if ( other != nullptr )
          {
            // Collide.
            handle_boat_collision(boat_collision_point, stern_collision_points[i][y], other, result);
            collided = (result.result_information == CommandResult::COLLISION);
          }
        }


      }
      if ( i < bow_collision_points.size() )
      {
        for ( int y = 0; y < bow_collision_points[i].size(); y++ )
        {
          Point boat_collision_point = get_bow().get_point_in_direction(get_opposite_direction(location.get_direction()), y);
          std::shared_ptr<ICollidable> other = collision_detector->collision_check(bow_collision_points[i][y]);
          if ( other != nullptr )
          {
            // Collide.
            handle_boat_collision(boat_collision_point, bow_collision_points[i][y], other, result);
            collided = (result.result_information == CommandResult::COLLISION);

          }
        }
      }
      if ( collided )
      {
        return result;
      }
    }

    Position new_position = Position(rotation_point.get_point_in_direction(get_opposite_direction(turning_orientation), center), turning_orientation);
    for ( int i = 0; i < get_size(); i++ )
    {
      Point boat_collision_point = location.get_point().get_point_in_direction(location.get_direction(), i);
      Point possible_collision = new_position.get_point().get_point_in_direction(new_position.get_direction(), i);
      if ( rotation_point == possible_collision )
      {
        continue;
      }
      std::shared_ptr<ICollidable> other = collision_detector->collision_check(possible_collision);
      if ( other != nullptr )
      {
        // Collide.
        handle_boat_collision(boat_collision_point, possible_collision, other, result);
        collided = result.result_information == CommandResult::COLLISION;
      }
    }

    if ( !collided )
    {
      location = new_position;
      last_pos = new_position;
    }
    else
    {
      //Collision
      break;
    }
    result.count++;
    rotation_units--;
  }
  return result;
};

CommandResult Boat::perform_action(const Command& cmd)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::UNKNOWN_COMMAND;
  bool action_performed = false;
  switch ( cmd.action )
  {
  case Action::MOVE_FORWARDS:
    result = move_forwards(cmd.pt);
    action_performed = true;
    break;
  case Action::MOVE_BACKWARDS:
    result = move_backwards(cmd.count);
    action_performed = true;
    break;
  case Action::SLIDE_LEFT:
  case Action::SLIDE_RIGHT:
    result = shift(cmd.rel_dir);
    action_performed = true;
    break;
  case Action::TURN_LEFT:
  case Action::TURN_RIGHT:
  case Action::TURN_BACK:
    result = rotate(cmd.rel_dir, cmd.count);
    action_performed = true;
    break;
  case Action::FIRE_CANNON:
    result = fire_cannon(cmd.pt);
    action_performed = true;
    break;
  case Action::FIRE_TORPEDO:
    result = fire_torpedo(cmd.random);
    action_performed = true;
    break;
  case Action::LAY_MINE:
    result = drop_mine(cmd.pt);
    action_performed = true;
    break;
  case Action::PICK_UP_MINE:
    result = pick_up_mine(cmd.pt);
    action_performed = true;
    break;
  case Action::REPAIR:
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

Unit::UnitClass Boat::get_unit_type() const
{
  return Unit::BOAT;
};

Point Boat::get_rotation_point() const
{
  Point rotation_point;
  switch ( location.get_direction() )
  {
  case NORTH:
    rotation_point = location.get_point().get_north_by(center);
    break;
  case SOUTH:
    rotation_point = location.get_point().get_south_by(center);
    break;
  case EAST:
    rotation_point = location.get_point().get_east_by(center);
    break;
  case WEST:
    rotation_point = location.get_point().get_west_by(center);
    break;
  }
  return rotation_point;
}

std::shared_ptr<Unit> Boat::s_construct_cruiser(const Position & p, const RgbColor & col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<Boat> b(new Boat(p, 2, 5, collision_detector, pmine_handler, 10, 1, 0, CRUISER));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(1, 1, 10, -1);
  b->capabilities.push_back(Action::Action_enum::FIRE_CANNON);
  b->init_cannon(5, 5, 5, 5, Cannon::HEAVY);
  return std::dynamic_pointer_cast<Unit, Boat>( b );
}

std::shared_ptr<Unit> Boat::s_construct_repair_boat(const Position & p, const RgbColor & col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<Boat> b(new Boat(p, 2, 2, collision_detector, pmine_handler, 6, 1, 0, REPAIR_BOAT));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(2, 2, 4, 2);
  b->capabilities.push_back(Action::Action_enum::REPAIR);
  return std::dynamic_pointer_cast<Unit, Boat>( b );
}

std::shared_ptr<Unit> Boat::s_construct_destroyer(const Position & p, const RgbColor & col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<Boat> b(new Boat(p, 1, 4, collision_detector, pmine_handler, 8, 1, 0, DESTROYER));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(1, 1, 8, -1);
  b->capabilities.push_back(Action::Action_enum::FIRE_CANNON);
  b->capabilities.push_back(Action::Action_enum::FIRE_TORPEDO);
  b->init_cannon(4, 4, 4, 4, Cannon::LIGHT);
  b->init_torpedo();
  return std::dynamic_pointer_cast<Unit, Boat>( b );
}

std::shared_ptr<Unit> Boat::s_construct_mine_layer(const Position & p, const RgbColor & col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<Boat> b(new Boat(p, 2, 2, collision_detector, pmine_handler, 6, 1, 0, MINE_LAYER));
  b->set_color(col);
  b->set_radar_level(Radar::Level::SONAR);
  b->set_radar_range(2, 2, 4, 2);
  b->capabilities.push_back(Action::Action_enum::FIRE_CANNON);
  b->capabilities.push_back(Action::Action_enum::LAY_MINE);
  b->capabilities.push_back(Action::Action_enum::PICK_UP_MINE);
  b->init_cannon(2, 2, 1, 1, Cannon::LIGHT);
  b->init_mine_dropper(5);
  return std::dynamic_pointer_cast<Unit, Boat>( b );
}

std::shared_ptr<Unit> Boat::s_construct_torpedo_boat(const Position & p, const RgbColor & col,
  ICollisionDetector* collision_detector, IMineHandler* pmine_handler)
{
  std::shared_ptr<Boat> b(new Boat(p, 1, 3, collision_detector, pmine_handler, 9, 2, 1, TORPEDO_BOAT));
  b->set_color(col);
  b->set_radar_level(Radar::Level::RADAR);
  b->set_radar_range(1, 1, 6, -1);
  b->capabilities.push_back(Action::Action_enum::FIRE_CANNON);
  b->capabilities.push_back(Action::Action_enum::FIRE_TORPEDO);
  b->init_cannon(2, 2, 2, 0, Cannon::LIGHT);
  b->init_torpedo();
  return std::dynamic_pointer_cast<Unit, Boat>( b );
}

JSONObject Boat::serialize() const
{
  JSONObject boat_serialize = Unit::serialize();
  boat_serialize.set_value("name", JSONElement_ptr(new JSONString("Boat")));
  boat_serialize.set_value("speed", JSONElement_ptr(new JSONString(std::to_string(speed))));
  boat_serialize.set_value("rotation_speed", JSONElement_ptr(new JSONString(std::to_string(rotation_speed))));
  boat_serialize.set_value("center", JSONElement_ptr(new JSONString(std::to_string(center))));
  boat_serialize.set_value("boat_type", JSONElement_ptr(new JSONString(std::to_string(boat_type))));

  if ( torpedo == nullptr )
  {
    boat_serialize.set_value("torpedo", JSONElement_ptr(new JSONString("NULL")));
  }
  else
  {
    boat_serialize.set_value("torpedo", JSONElement_ptr(new JSONObject(( *torpedo ).serialize())));
  }
  if ( cannon == nullptr )
  {
    boat_serialize.set_value("cannon", JSONElement_ptr(new JSONString("NULL")));
  }
  else
  {
    boat_serialize.set_value("cannon", JSONElement_ptr(new JSONObject(( *cannon ).serialize())));
  }
  if ( mine_dropper == nullptr )
  {
    boat_serialize.set_value("mine_dropper", JSONElement_ptr(new JSONString("NULL")));
  }
  else
  {
    boat_serialize.set_value("mine_dropper", JSONElement_ptr(new JSONObject(( *mine_dropper ).serialize())));
  }
  return boat_serialize;
}


Boat Boat::unserialize(JSONObject object, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler)
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
  BoatType new_boat_type = static_cast<BoatType>( atoi(JSONString::extract_string(object["boat_type"]).c_str()) );
  int new_length = atoi(JSONString::extract_string(object["length"]).c_str());
  int new_speed = atoi(JSONString::extract_string(object["speed"]).c_str());
  int new_rotation_speed = atoi(JSONString::extract_string(object["rotation_speed"]).c_str());
  int new_center = atoi(JSONString::extract_string(object["center"]).c_str());
  std::shared_ptr<Torpedo> new_torpedo = nullptr;
  std::shared_ptr<Cannon> new_cannon = nullptr;
  std::shared_ptr<MineDropper> new_mine_dropper = nullptr;

  JSONElement_ptr new_torpedo_json_ptr = object["torpedo"];
  if ( JSONObject::is_json_object(new_torpedo_json_ptr) )
  {
    new_torpedo = std::shared_ptr<Torpedo>(new Torpedo(Torpedo::unserialize(dynamic_cast<JSONObject&> ( *new_torpedo_json_ptr ))));
  }

  JSONElement_ptr new_cannon_json_ptr = object["cannon"];
  if ( JSONObject::is_json_object(new_cannon_json_ptr) )
  {
    new_cannon = std::shared_ptr<Cannon>(new Cannon(Cannon::unserialize(dynamic_cast<JSONObject&> ( *new_cannon_json_ptr ))));
  }

  JSONElement_ptr new_mine_dropper_json_ptr = object["mine_dropper"];
  if ( JSONObject::is_json_object(new_mine_dropper_json_ptr) )
  {
    new_mine_dropper = std::shared_ptr<MineDropper>(new MineDropper(MineDropper::unserialize(dynamic_cast<JSONObject&> ( *new_mine_dropper_json_ptr ), pmine_handler)));
  }

  //Create Unit object (can replace so that a special constructor or static method is used to create it)
  Boat b(new_id, new_maximum_health, new_cells, new_location, p_collision_detector, pmine_handler, new_speed, new_rotation_speed, new_center, new_boat_type);

  b.capabilities = new_capabilities;
  b.type = new_type;
  b.color = new_color;
  b.radar = new_radar;
  b.mine_dropper = new_mine_dropper;
  b.torpedo = new_torpedo;
  b.cannon = new_cannon;

  return b;
}

void Boat::init_cannon(int pright, int pleft, int pfront, int pback, Cannon::CannonType type)
{
  if ( type == Cannon::LIGHT )
  {
    cannon = std::shared_ptr<Cannon>(new Cannon(Cannon::get_default_cannon(pright, pleft, pfront, pback)));
  }
  else if ( type == Cannon::HEAVY )
  {
    cannon = std::shared_ptr<Cannon>(new Cannon(Cannon::get_default_heavy_cannon(pright, pleft, pfront, pback)));
  }
}

void Boat::init_mine_dropper(int pnum_of_mines)
{
  mine_dropper = std::shared_ptr<MineDropper>(new MineDropper(MineDropper::get_default_mine_dropper(pnum_of_mines, mine_handler)));
}

void Boat::init_torpedo()
{
  torpedo = std::shared_ptr<Torpedo>(new Torpedo(Torpedo::get_default_torpedo()));
}

bool Boat::has_mines() const
{
  if ( mine_dropper != nullptr )
  {
    return ( *mine_dropper ).has_mines_left();
  }
  return false;
}

bool Boat::has_mine_layer() const
{
  return (mine_dropper != nullptr);
}

int Boat::get_num_mines_left() const
{
  if ( mine_dropper != nullptr )
  {
    return ( *mine_dropper ).get_number_of_mines_left();
  }
  return -1;
}


CommandResult Boat::fire_torpedo(int prand)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;
  result.result_information = CommandResult::MISS;
  Point bow = get_bow();
  if ( torpedo != nullptr )
  {
    FiredTorpedo ftorpedo = ( *torpedo ).fire_torpedo(Position(get_bow(), get_direction()), ( *torpedo ).get_max_distance());
    Point cur_point = ftorpedo.get_current_position().get_point();
    while ( ftorpedo.can_advance() )
    {
      ftorpedo.advance_torpedo();
      cur_point = ftorpedo.get_current_position().get_point();
      std::shared_ptr<ICollidable> other = collision_detector->collision_check(cur_point);
      if ( other != nullptr )
      {
        handle_explosive_collision(&ftorpedo, cur_point, other, prand);
        result.result_information = CommandResult::HIT;
        result.result_point = cur_point;
        result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(cur_point, other));
        break;
      }
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::NO_TORPEDO;
    result.result_information = CommandResult::NO_INFORMATION;
  }
  return result;
}

CommandResult Boat::fire_cannon(Point target)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;
  result.result_information = CommandResult::MISS;
  result.result_point = target;

  if ( cannon != nullptr )
  {
    if ( ( *cannon ).is_in_range(get_position(), get_size(), target) )
    {
      CannonShell cs = ( *cannon ).fire_cannon(target);
      std::shared_ptr<ICollidable> other = collision_detector->collision_check(cs.get_fired_at_point());
      if ( other != nullptr )
      {
        handle_explosive_collision(&cs, cs.get_fired_at_point(), other, 0);
        result.result_information = CommandResult::HIT;
        result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(target, other));
      }
    }
    else
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::OUT_OF_RANGE;
      result.result_information = CommandResult::NO_INFORMATION;
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    result.result_information = CommandResult::NO_INFORMATION;
    result.error_type = CommandResult::NO_CANNON;
  }
  result.result_point = target;
  return result;
}

CommandResult Boat::drop_mine(Point target)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.result_information = CommandResult::NO_INFORMATION;
  result.error_type = CommandResult::NO_ERROR;
  result.result_point = target;

  if ( mine_dropper != nullptr )
  {
    if ( !( *mine_dropper ).has_mines_left() )
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::NO_MINES_LEFT;
      return result;
    }
    if (mine_dropper->is_in_range(get_position(), get_size(), target) )
    {
      if (mine_dropper->drop_mine(target))
      {
        result.result_type = CommandResult::SUCCESS;
        result.error_type = CommandResult::NO_ERROR;
      }
      else
      {
        //MUST NOT BE ABLE TO DROP A MINE DIRECTLY ON A BOAT
        result.result_type = CommandResult::ERROR;
        result.error_type = CommandResult::CANNOT_DROP_MINE;
        //result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(target, other));
      }
    }
    else
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::OUT_OF_RANGE;
      result.result_information = CommandResult::NO_INFORMATION;
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::NO_MINE_DROPPER;
    result.result_information = CommandResult::NO_INFORMATION;
  }
  result.result_point = target;
  return result;
}

CommandResult Boat::pick_up_mine(Point pick_up_point)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.result_information = CommandResult::NO_INFORMATION;
  result.result_point = pick_up_point;

  if ( mine_dropper != nullptr )
  {
    if ( ( *mine_dropper ).is_in_range(get_position(), get_size(), pick_up_point) )
    {
      if ( !( *mine_dropper ).pick_up_mine(pick_up_point) )
      {
        result.result_type = CommandResult::ERROR;
        result.error_type = CommandResult::NO_MINE_TO_PICKUP;
        result.result_information = CommandResult::NO_INFORMATION;
      }
      else
      {
        result.result_type = CommandResult::SUCCESS;
        result.error_type = CommandResult::NO_ERROR;
      }
    }
    else
    {
      result.result_type = CommandResult::ERROR;
      result.error_type = CommandResult::OUT_OF_RANGE;
      result.result_information = CommandResult::NO_INFORMATION;
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    result.error_type = CommandResult::NO_MINE_DROPPER;
    result.result_information = CommandResult::NO_INFORMATION;
  }
  result.result_point = pick_up_point;
  return result;
}

CommandResult Boat::repair_boat(Point target)
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
    else if ( ICollidable::extract_type(other) == ICollidableType::BASE )
    {
      dynamic_cast<Unit&>( *other ).heal_damage(1);
      result.count = 1;
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

std::array<std::array<bool, 30>, 30> Boat::get_torpedo_range() const
{
  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> range;
  range.fill(row);
  if ( torpedo != nullptr )
  {
    std::vector<Point> torpedo_range = ( *torpedo ).get_weapon_range(get_position(), get_size());
    for ( auto it = torpedo_range.begin(); it != torpedo_range.end(); ++it )
    {
      if ( !is_out_of_bounds(*it) )
      {
        range[( *it ).get_x()][( *it ).get_y()] = true;
      }
    }
  }
  return range;
}

std::array<std::array<bool, 30>, 30> Boat::get_repair_range() const
{
  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> range;
  range.fill(row);
  std::vector<Point> repair_range = get_points_in_range(location, get_length(), 1, 1, 1, 1);
  for ( auto it = repair_range.begin(); it != repair_range.end(); ++it )
  {
    if ( !is_out_of_bounds(*it) )
    {
      range[( *it ).get_x()][( *it ).get_y()] = true;
    }
  }

  return range;
}

std::array<std::array<bool, 30>, 30> Boat::get_drop_pick_mine_range() const
{
  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> range;
  range.fill(row);
  if ( mine_dropper != nullptr )
  {
    std::vector<Point> mine_dropper_range = ( *mine_dropper ).get_weapon_range(get_position(), get_size());
    for ( auto it = mine_dropper_range.begin(); it != mine_dropper_range.end(); ++it )
    {
      if ( !is_out_of_bounds(*it) )
      {
        range[( *it ).get_x()][( *it ).get_y()] = true;
      }
    }
  }
  return range;
}

std::array<std::array<bool, 30>, 30> Boat::get_cannon_range() const
{
  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> range;
  range.fill(row);
  if ( cannon != nullptr )
  {
    std::vector<Point> cannon_range = ( *cannon ).get_weapon_range(get_position(), get_size());
    for ( auto it = cannon_range.begin(); it != cannon_range.end(); ++it )
    {
      if ( !is_out_of_bounds(*it) )
      {
        range[( *it ).get_x()][( *it ).get_y()] = true;
      }
    }

  }
  return range;
}

CommandResult Boat::handle_explosive_collision(Explosive* exp, Point collision_point, std::shared_ptr<ICollidable> colliding_object, int prand)
{
  CommandResult result(get_id());
  result.result_type = CommandResult::SUCCESS;
  result.result_information = CommandResult::HIT;
  result.error_type = CommandResult::NO_ERROR;
  result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(collision_point, colliding_object));

  if ( Unit::is_unit(colliding_object.get()) )
  {
    Unit* colliding_unit = (Unit *)colliding_object.get();

    if ( FiredTorpedo::is_fired_torpedo(exp) )
    {
      FiredTorpedo* ftorpedo = (FiredTorpedo *)exp;
      Direction torpedo_dir = ftorpedo->get_current_position().get_direction();
      Direction unit_dir = colliding_unit->get_direction();
      //If hit by the side we must damage an extra cell
      if ( unit_dir != torpedo_dir && torpedo_dir != get_opposite_direction(unit_dir) )
      {
        std::vector<Point> unit_points = colliding_unit->get_points();
        auto it = std::find(unit_points.begin(), unit_points.end(), collision_point);

        //Find an extra cell to damage near the collision point
        int index = std::distance(unit_points.begin(), it);
        int random = ( prand % 2 ) == 0 ? -1 : 1;

        bool damaged_extra = false;
        //if a valid cell
        if ( ( index - random ) >= 0 )
        {
          //if it is not destroyed damage it
          if ( !colliding_unit->get_cell_x_cells_away_from_back(index - random).is_destroyed() )
          {
            colliding_unit->collide_at(*( it - random ), ftorpedo->get_damage() / 2);
            damaged_extra = true;
          }

        }
        //if a valid cell and a cell has not been already damaged
        if ( ( index + random ) >= 0 && ( index + random ) < colliding_unit->get_size() && !damaged_extra )
        {
          if ( !colliding_unit->get_cell_x_cells_away_from_back(index + random).is_destroyed() )
          {
            colliding_unit->collide_at(*( it + random ), ftorpedo->get_damage() / 2);
          }
        }

      }
      colliding_unit->collide_at(collision_point, ftorpedo->get_damage());
    }
    else if ( CannonShell::is_cannon_shell(exp) )
    {
      colliding_unit->collide_at(collision_point, exp->get_damage());
    }
  }
  else if ( Mine::is_mine(colliding_object.get()) )
  {
    std::shared_ptr<Mine> m = std::dynamic_pointer_cast<Mine>(colliding_object);
    mine_handler->remove_mine(m->get_point());
  }
  return result;
}

void Boat::handle_boat_collision(Point boat_collision_point, Point collision_point, std::shared_ptr<ICollidable> colliding_object, CommandResult& command_result)
{
  command_result.result_type = CommandResult::SUCCESS;
  command_result.result_information = CommandResult::COLLISION;
  command_result.error_type = CommandResult::NO_ERROR;

  if ( Mine::is_mine(colliding_object.get()) && mine_dropper == nullptr )
  {
    Mine& collided = dynamic_cast<Mine&>(*colliding_object);
    collide_at(boat_collision_point, collided.get_damage());
    std::vector<Point> boat_points = get_points();

    //start from bow
    std::reverse(boat_points.begin(), boat_points.end());
    int index = boat_points.size() - 1;
    for ( auto it = boat_points.begin(); it != boat_points.end(); it++ )
    {
      if ( boat_collision_point != ( *it ) && !( get_cell_x_cells_away_from_back(index).is_destroyed() ) )
      {
        collide_at(*it, collided.get_damage());
        break;
      }
      index--;
    }
    mine_handler->remove_mine(collided.get_point());
  }
  else if (Mine::is_mine(colliding_object.get()))
  {
    // In this case a minelayer is potentially intersecting a mine.
    std::shared_ptr<Mine> m = std::dynamic_pointer_cast<Mine>(colliding_object);
    if (collision_point != m->get_point())
    {
      command_result.result_information = CommandResult::NO_INFORMATION;
    }
    else
    {
      command_result.result_type = CommandResult::ERROR;
      command_result.error_type = CommandResult::CANNOT_MOVE_AT_SPECIFIED_POINT;
    }
  }
  if (command_result.result_type == CommandResult::SUCCESS && command_result.result_information == CommandResult::COLLISION)
  {
    command_result.result_collidables.push_back(std::pair<Point, std::shared_ptr<ICollidable>>(collision_point, colliding_object));
  }
}

Boat::BoatType Boat::get_boat_type() const
{
  return boat_type;
}

std::string Boat::convert_boat_type_to_string(Boat::BoatType ptype)
{
  std::string sboat_type = "";
  if ( ptype == BoatType::CRUISER )
  {
    sboat_type = "Cruiser";
  }
  else if ( ptype == BoatType::DESTROYER )
  {
    sboat_type = "Destroyer";
  }
  else if ( ptype == BoatType::MINE_LAYER )
  {
    sboat_type = "Mine Layer";
  }
  else if ( ptype == BoatType::RADAR_BOAT )
  {
    sboat_type = "Radar Boat";
  }
  else if ( ptype == BoatType::REPAIR_BOAT )
  {
    sboat_type = "Repair Boat";
  }
  else if ( ptype == BoatType::TORPEDO_BOAT )
  {
    sboat_type = "Torpedo Boat";
  }
  return sboat_type;
}
