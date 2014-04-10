#include "GameState.h"
#include "ReefGenerator.h"
#include "KamikazeBoat.h"
#include <algorithm>

GameState::GameState()
{};

GameState::GameState(JSONObject object)
{
  PlayerData pone = PlayerData::unserialize(dynamic_cast<JSONObject&> ( *object["player_one"] ), *this);
  PlayerData ptwo = PlayerData::unserialize(dynamic_cast<JSONObject&> ( *object["player_two"] ), *this);

  std::vector<std::shared_ptr<Reef>> new_reefs;
  std::vector<JSONElement_ptr> reef_json_element_list = dynamic_cast<JSONArray&> ( *object["reefs"] ).get_elements();
  for ( auto it = reef_json_element_list.begin(); it != reef_json_element_list.end(); it++ )
  {
    new_reefs.push_back(std::shared_ptr<Reef>(new Reef(Reef::unserialize(dynamic_cast<JSONObject&> ( **it )))));
  }

  std::vector<std::shared_ptr<Mine>> new_mines;
  std::vector<JSONElement_ptr> mine_json_element_list = dynamic_cast<JSONArray&> ( *object["mines"] ).get_elements();
  for ( auto it = mine_json_element_list.begin(); it != mine_json_element_list.end(); it++ )
  {
    new_mines.push_back(std::shared_ptr<Mine>(new Mine(Mine::unserialize(dynamic_cast<JSONObject&> ( **it )))));
  }

  game_id = atoi(JSONString::extract_string(object["game_id"]).c_str());
  player_one = pone;
  player_two = ptwo;
  reefs = new_reefs;
  mines = new_mines;

}

GameState::~GameState()
{};


/* Collision would involve running a test against elements of both vectors of
 * boats, as well as against both bases and the vector of collidable objects,
 * such as reef units or mines.
 */

void GameState::set_initial_state()
{
  game_id = 0;
  player_one.color = RgbColor(255, 255, 0);
  player_two.color = RgbColor(0, 255, 0);
  player_one.home_base = std::dynamic_pointer_cast<Base, Unit>( Base::s_construct_base(Position(Point(0, 10), Direction::SOUTH), player_one.color, this) );

  player_one.units.push_back(KamikazeBoat::s_construct_kamikaze_boat(Position(Point(0, 9), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_cruiser(Position(Point(1, 10), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_cruiser(Position(Point(1, 11), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_destroyer(Position(Point(1, 12), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_destroyer(Position(Point(1, 13), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_destroyer(Position(Point(1, 14), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_torpedo_boat(Position(Point(1, 15), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_torpedo_boat(Position(Point(1, 16), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_mine_layer(Position(Point(1, 17), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_mine_layer(Position(Point(1, 18), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(RadarBoat::s_construct_radar_boat(Position(Point(1, 19), Direction::EAST), player_one.color, this, this));
  player_one.units.push_back(Boat::s_construct_repair_boat(Position(Point(0, 20), Direction::EAST), player_one.color, this, this));
  player_two.home_base = std::dynamic_pointer_cast<Base, Unit>( Base::s_construct_base(Position(Point(29, 10), Direction::SOUTH), player_two.color, this) );

  player_two.units.push_back(KamikazeBoat::s_construct_kamikaze_boat(Position(Point(29, 9), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_cruiser(Position(Point(28, 10), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_cruiser(Position(Point(28, 11), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_destroyer(Position(Point(28, 12), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_destroyer(Position(Point(28, 13), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_destroyer(Position(Point(28, 14), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_torpedo_boat(Position(Point(28, 15), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_torpedo_boat(Position(Point(28, 16), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_mine_layer(Position(Point(28, 17), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_mine_layer(Position(Point(28, 18), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(RadarBoat::s_construct_radar_boat(Position(Point(28, 19), Direction::WEST), player_two.color, this, this));
  player_two.units.push_back(Boat::s_construct_repair_boat(Position(Point(29, 20), Direction::WEST), player_two.color, this, this));
}

std::vector<Reef> GameState::set_new_reef_config()
{
  reefs.clear();
  std::vector<Reef> reef_config;
  for ( auto p : ReefGenerator::generate_reef(10, 24, 0.1) )
  {
    reefs.push_back(std::shared_ptr<Reef>(new Reef(Point(p.get_x() + 10, p.get_y() + 3))));
    reef_config.push_back(Reef(Point(p.get_x() + 10, p.get_y() + 3)));
  }
  return reef_config;
}

void GameState::set_reef_config(std::vector<Reef> preefs)
{
  reefs.clear();
  for ( auto it = preefs.begin(); it != preefs.end(); it++ )
  {
    reefs.push_back(std::shared_ptr<Reef>(new Reef(*it)));
  }
}


CommandResult GameState::deliver_command(const Command& cmd)
{
  CommandResult result(cmd.id);
  result.result_type = CommandResult::ERROR;
  result.error_type = CommandResult::UNKNOWN_COMMAND;
  // iterate over all the units and attempt to execute the command on each one
  // First, player one:
  if ( cmd.action == Action::SWAP_BOATS )
  {
    result = swap_boats(cmd.id, cmd.pt);
    return result;
  }
  for ( std::shared_ptr<Unit> u : player_one.units )
  {
    if ( u->get_id() == cmd.id )
    {
      result = u->perform_action(cmd);
      break;
    }
  }
  if ( player_one.home_base->get_id() == cmd.id )
  {
    result = player_one.home_base->perform_action(cmd);
  }
  // Next, player two:
  for ( std::shared_ptr<Unit> u : player_two.units )
  {
    if ( u->get_id() == cmd.id )
    {
      result = u->perform_action(cmd);
      break;
    }
  }
  if ( player_two.home_base->get_id() == cmd.id )
  {
    result = player_two.home_base->perform_action(cmd);
  }
  result.action = cmd.action;

  // This is sort of an ugly way to handle a relatively complex sequence of events.
  // Prefer a cleaner solution.
  if (result.result_type == CommandResult::ResultType::SUCCESS)
  {
    Player::Side ps;
    result.game_won = is_won(ps);
  }
  // If command_delivered is false, either an invalid action was attempted or
  // an invalid unit was specified!  Neither of those is an expected behaviour,
  // so there should be an error message generated here.

  return result;
}

std::shared_ptr<ICollidable> GameState::collision_check(const Point& pt)
{
  std::shared_ptr<ICollidable> colliding(nullptr);

  if ( colliding == nullptr )
  {
    for ( std::shared_ptr<Unit> b : player_one.units )
    {
      if ( b->contains_point(pt) && !b->is_sunk() )
      {
        colliding = std::shared_ptr<ICollidable>(b);
        break;
      }
    }
  }
  // Check for player_one collisions
  
  if ( colliding == nullptr )
  {
    if ( player_one.home_base->will_collide(pt) && !player_one.home_base->is_sunk() )
    {
      colliding = std::shared_ptr<ICollidable>(player_one.home_base);
    }
  }
  // Check for player_two collisions
  if ( colliding == nullptr )
  {
    for ( std::shared_ptr<Unit> b : player_two.units )
    {
      if ( b->contains_point(pt) && !b->is_sunk() )
      {
        colliding = std::shared_ptr<ICollidable>(b);
        break;
      }
    }
  }
  // Player two base
  if ( colliding == nullptr )
  {
    if ( player_two.home_base->will_collide(pt) && !player_two.home_base->is_sunk() )
    {
      colliding = std::shared_ptr<ICollidable>(player_two.home_base);
    }
  }
  if ( colliding == nullptr )
  {
    // Check for mine collisions
    for ( std::shared_ptr<Mine> c : mines )
    {
      if ( ( *c ).will_collide(pt) )
      {
        colliding = std::shared_ptr<ICollidable>(c);
        break;
      }
    }
  }
  // Check for reef collisions
  if ( colliding == nullptr )
  {
    for ( std::shared_ptr<Reef> r : reefs )
    {
      if ( ( *r ).will_collide(pt) )
      {
        colliding = std::shared_ptr<ICollidable>(r);
        break;
      }
    }
  }
  return colliding;
};

std::array<std::array<Radar::Level, 30>, 30> GameState::get_visibility(Player::Side side) const
{
  std::array<std::array<Radar::Level, 30>, 30> board;
  board.fill(std::array<Radar::Level, 30>());
  for ( auto row : board )
  {
    row.fill(Radar::Level::NONE);
  }

  for ( Reef & r : get_reefs() )
  {
    const Point & loc = r.get_location();
    if ( board[loc.get_x()][loc.get_y()] == Radar::Level::NONE )
    {
      board[loc.get_x()][loc.get_y()] = Radar::Level::SONAR;
    }
  }

  for ( int i = 0; i < 10; i++ )
  {
    board[0][10 + i] = Radar::Level::SONAR;
    board[29][10 + i] = Radar::Level::SONAR;
  }

  if ( side == Player::Side::PLAYER_ONE )
  {
    for ( std::shared_ptr<Unit> b : player_one.units )
    {
      if ( !b->is_sunk() )
      {
        b->add_visibility(board);
      }
    }
    if ( !player_one.home_base->is_sunk() )
    {
      player_one.home_base->add_visibility(board);
    }
  }
  else if ( side == Player::Side::PLAYER_TWO )
  {
    for ( std::shared_ptr<Unit> b : player_two.units )
    {
      if ( !b->is_sunk() )
      {
        b->add_visibility(board);
      }
    }
    if ( !player_two.home_base->is_sunk() )
    {
      player_two.home_base->add_visibility(board);
    }
  }
  else
  {
    std::array<Radar::Level, 30> row;
    row.fill(Radar::Level::SONAR);
    board.fill(row);
  }
  return board;
};

const PlayerData & GameState::get_player_one() const
{
  return player_one;
}
const PlayerData & GameState::get_player_two() const
{
  return player_two;
}

std::vector<Reef> GameState::get_reefs() const
{
  std::vector<Reef> all_reefs;
  for ( auto it = reefs.begin(); it != reefs.end(); it++ )
  {
    all_reefs.push_back(**it);
  }
  return all_reefs;
}
std::vector<Mine> GameState::get_mines() const
{
  std::vector<Mine> all_mines;
  for ( auto it = mines.begin(); it != mines.end(); it++ )
  {
    all_mines.push_back(**it);
  }
  return all_mines;
}

bool GameState::is_won(Player::Side& player) const
{
  bool rv = false;
  if (std::all_of(player_one.units.begin(), player_one.units.end(), [](std::shared_ptr<Unit> u) { return u->is_sunk(); }))
  {
    rv = true;
    player = Player::PLAYER_TWO;
  }
  else if (std::all_of(player_two.units.begin(), player_two.units.end(), [](std::shared_ptr<Unit> u) { return u->is_sunk(); }))
  {
    rv = true;
    player = Player::PLAYER_ONE;
  }
  return rv;
};

JSONObject GameState::serialize() const
{
  JSONObject gamestate_serialize = JSONObject::get_template("Class", "GameState");

  gamestate_serialize.set_value("game_id", JSONElement_ptr(new JSONString(std::to_string(game_id))));

  gamestate_serialize.set_value("player_one", JSONElement_ptr(new JSONObject(player_one.serialize())));
  gamestate_serialize.set_value("player_two", JSONElement_ptr(new JSONObject(player_two.serialize())));

  JSONArray reefs_array;
  for ( auto it = reefs.begin(); it != reefs.end(); it++ )
  {
    reefs_array.add(JSONElement_ptr(new JSONObject(( **it ).serialize())));
  }
  gamestate_serialize.set_value("reefs", JSONElement_ptr(new JSONArray(reefs_array)));

  JSONArray mines_array;
  for ( auto it = mines.begin(); it != mines.end(); it++ )
  {
    mines_array.add(JSONElement_ptr(new JSONObject(( **it ).serialize())));
  }
  gamestate_serialize.set_value("mines", JSONElement_ptr(new JSONArray(mines_array)));

  return gamestate_serialize;
}

JSONObject PlayerData::serialize() const
{
  JSONObject playerdata_serialize = JSONObject::get_template("Class", "PlayerData");
  playerdata_serialize.set_value("home_base", JSONElement_ptr(new JSONObject(home_base->serialize())));
  playerdata_serialize.set_value("color", JSONElement_ptr(new JSONObject(color.serialize())));

  JSONArray units_array;
  for ( auto it = units.begin(); it != units.end(); it++ )
  {
    units_array.add(JSONElement_ptr(new JSONObject(( **it ).serialize())));
  }
  playerdata_serialize.set_value("units", JSONElement_ptr(new JSONArray(units_array)));

  return playerdata_serialize;
}


PlayerData PlayerData::unserialize(JSONObject object, GameState& state)
{
  std::vector<std::shared_ptr<Unit>> new_units;
  std::vector<JSONElement_ptr> unit_json_element_list = dynamic_cast<JSONArray&> ( *object["units"] ).get_elements();

  for ( auto it = unit_json_element_list.begin(); it != unit_json_element_list.end(); it++ )
  {
    std::shared_ptr<Unit> unit_ptr;
    JSONObject unit_json = dynamic_cast< JSONObject& > ( **it );
    std::string unit_type = JSONString::extract_string(unit_json["name"]);

    //I don't like this!!!
    if ( unit_type.compare("Boat") == 0 )
    {
      new_units.push_back(std::shared_ptr<Unit>(new Boat(Boat::unserialize(unit_json, &state, &state))));
    }
    else if ( unit_type.compare("RadarBoat") == 0 )
    {
      new_units.push_back(std::shared_ptr<Unit>(new RadarBoat(RadarBoat::unserialize(unit_json, &state, &state))));
    }
    else if ( unit_type.compare("KamikazeBoat") == 0 )
    {
      new_units.push_back(std::shared_ptr<Unit>(new KamikazeBoat(KamikazeBoat::unserialize(unit_json, &state, &state))));
    }
    else if ( unit_type.compare("Unit") == 0 )
    {
      new_units.push_back(std::shared_ptr<Unit>(new Unit(Unit::unserialize(unit_json, &state))));
    }

  }

  Base new_base = Base::unserialize(dynamic_cast<JSONObject&> ( *object["home_base"] ), &state);
  RgbColor new_color = RgbColor::unserialize(dynamic_cast<JSONObject&> ( *object["color"] ));

  PlayerData pdata;
  pdata.units = new_units;
  pdata.home_base = std::shared_ptr<Base>(new Base(new_base));
  pdata.color = new_color;

  return pdata;
}

bool GameState::is_mine_at(const Point& p)
{
  // Check for mine collisions
  for ( std::shared_ptr<Mine> c : mines )
  {
    if ( ( *c ).will_collide(p) )
    {
      return true;
    }
  }
  return false;
}

bool GameState::add_mine(const Mine& mine)
{
  auto mine_test = [](GameState *gs, const Point& p)
  {
    bool return_value = true; // By default safe to place a mine.
    if ( gs->is_mine_at(p) )
    {
      return_value = true;
    }
    std::shared_ptr<ICollidable> coll = gs->collision_check(p);
    if (coll != nullptr)
    {
      return_value = false; // If there is anything that can collide, do not place the mine.
      if (ICollidable::extract_type(coll) == ICollidable::BOAT)
      {
        std::shared_ptr<Boat> b = std::dynamic_pointer_cast<Boat>(coll);
        if (b->has_mine_layer())
        {
          return_value = true; // If the collidable is actually a minelayer, it's okay.
        }
      }
    }
    return return_value;
  };
  bool ret_val = (collision_check(mine.get_point()) == nullptr);
  if (ret_val)
  {
    ret_val = mine_test(this, mine.get_point().get_north_by(1));
  }
  if (ret_val)
  {
    ret_val = mine_test(this, mine.get_point().get_south_by(1));
  }
  if (ret_val)
  {
    ret_val = mine_test(this, mine.get_point().get_east_by(1));
  }
  if (ret_val)
  {
    ret_val = mine_test(this, mine.get_point().get_west_by(1));
  }
  if (ret_val)
  {
    mines.push_back(std::shared_ptr<Mine>(new Mine(mine)));
  }
  return ret_val;
}

bool GameState::remove_mine(const Point& mine_at)
{
  auto it = mines.begin();
  for ( ; it != mines.end(); it++ )
  {
    if ( ( **it ).get_point() == mine_at )
    {
      break;
    }
  }
  if ( it != mines.end() )
  {
    mines.erase(it);
    return true;
  }
  return false;
}

std::shared_ptr<Unit> GameState::get_unit_by_id(int pid)
{
  if ( player_one.home_base->get_id() == pid )
  {
    return player_one.home_base;
  }

  if ( player_two.home_base->get_id() == pid )
  {
    return player_two.home_base;
  }

  for ( auto it = player_one.units.begin(); it != player_one.units.end(); it++ )
  {
    if ( ( *it )->get_id() == pid )
    {
      return ( *it );
    }
  }
  for ( auto it = player_two.units.begin(); it != player_two.units.end(); it++ )
  {
    if ( ( *it )->get_id() == pid )
    {
      return ( *it );
    }
  }

  return nullptr;
}

CommandResult GameState::swap_boats(int boat_id, Point position)
{
  CommandResult result(boat_id);
  result.result_type = CommandResult::SUCCESS;
  result.error_type = CommandResult::NO_ERROR;
  result.result_information = CommandResult::NO_INFORMATION;
  std::shared_ptr<Boat> unit = std::dynamic_pointer_cast<Boat, Unit>(get_unit_by_id(boat_id));
  //This is a hack
  std::shared_ptr<Boat> boat = std::dynamic_pointer_cast<Boat, ICollidable>(collision_check(position));
  if ( unit != nullptr && boat != nullptr )
  {
    if ( (std::find(player_one.units.begin(), player_one.units.end(), unit) != player_one.units.end()
      && std::find(player_one.units.begin(), player_one.units.end(), boat) != player_one.units.end())
      || ( std::find(player_two.units.begin(), player_two.units.end(), unit) != player_two.units.end()
      && std::find(player_two.units.begin(), player_two.units.end(), boat) != player_two.units.end() ))
    {
      Position unit_pos = unit->get_position();
      Position boat_pos = boat->get_position();
      unit->set_position(boat_pos);
      boat->set_position(unit_pos);
    }
    else
    {
      result.result_type = CommandResult::ERROR;
    }
  }
  else
  {
    result.result_type = CommandResult::ERROR;
    //add error type
  }
  return result;
}