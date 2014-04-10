#include "GameRenderer.h"
#include "Point.h"
#include "Position.h"
#include "Angles.h"
#include "Radar.h"
#include "utilities.h"
#include "KamikazeBoat.h"

GameRenderer::GameRenderer()
{
  selected_unit = nullptr;
  ocean_dark.load_from_file("world/water_dark.png");
  ocean_light.load_from_file("world/water_light.png");
  boat_body.load_from_file("player/body.png");
  boat_head.load_from_file("player/head.png");
  base.load_from_file("player/base.png");
  reef.load_from_file("world/coral_reef.png");
  mine.load_from_file("weapon/mine.png");
  fog.load_from_file("world/fog.png");
  body_damaged.load_from_file("status/body_damaged.png");
  head_damaged.load_from_file("status/head_damaged.png");
  body_destroyed.load_from_file("status/body_destroyed.png");
  head_destroyed.load_from_file("status/head_destroyed.png");
  sunk.load_from_file("status/sunk.png");
  highlight_radar_range.load_from_file("highlight/basic.png");
  highlight_selected_unit.load_from_file("highlight/basic.png");
  highlight_selected_unit.set_color(5, 5, 5);
  highlight_weapon_range.load_from_file("highlight/basic.png");
  highlight_weapon_range.set_color(255, 0, 0);
  highlight_mine_pickup.load_from_file("highlight/basic.png");
  highlight_mine_pickup.set_color(0, 0, 255);
  diff_radar.load_from_file("diff/radar.png");
  diff_repair.load_from_file("diff/repair.png");
  diff_torpedo.load_from_file("diff/torpedo.png");
  diff_mine_layer.load_from_file("diff/mine.png");
}


GameRenderer::~GameRenderer()
{}

void GameRenderer::render_game_state(const GameState & gs, const Player::Side & side, const Action::Action_enum & selected_action)
{
  board = gs.get_visibility(side);
  render_background();
  //render bases first
  render_base(*gs.get_player_one().home_base);
  render_base(*gs.get_player_two().home_base);

  //render all sunken boats
  for ( auto b : gs.get_player_one().units )
  {
    if ( b->get_unit_type() == Unit::BOAT )
    {
      auto boat_ptr = ( std::dynamic_pointer_cast<Boat, Unit>( b ) );
      if ( boat_ptr->is_sunk() )
      {
        render_boat(*boat_ptr);
      }
    }
  }

  for ( auto b : gs.get_player_two().units )
  {
    if ( b->get_unit_type() == Unit::BOAT )
    {
      auto boat_ptr = ( std::dynamic_pointer_cast<Boat, Unit>( b ) );
      if ( boat_ptr->is_sunk() )
      {
        render_boat(*boat_ptr);
      }
    }
  }

  //render non sunken boats

  for ( auto b : gs.get_player_one().units )
  {
    if ( b->get_unit_type() == Unit::BOAT )
    {
      auto boat_ptr = ( std::dynamic_pointer_cast<Boat, Unit>( b ) );
      if ( !boat_ptr->is_sunk() )
      {
        render_boat(*boat_ptr);
      }
    }
  }

  for ( auto b : gs.get_player_two().units )
  {
    if ( b->get_unit_type() == Unit::BOAT )
    {
      auto boat_ptr = ( std::dynamic_pointer_cast<Boat, Unit>( b ) );
      if ( !boat_ptr->is_sunk() )
      {
        render_boat(*boat_ptr);
      }
    }
  }

  for ( auto rf : gs.get_reefs() )
  {
    render_reef(rf);
  }
  render_hidden(gs, side);
}

void GameRenderer::render_background()
{
  int number_of_tiles = 30 * 30;
  for ( int i = 0; i < number_of_tiles; ++i )
  {
    int x = i % 30;
    int y = i / 30;
    switch ( board[x][y] )
    {
    case Radar::Level::SONAR:
      ocean_light.set_position_on_grid(x, y);
      ocean_light.render();
      break;
    case Radar::Level::RADAR:
      ocean_dark.set_position_on_grid(x, y);
      ocean_dark.render();
      break;
    }
  }
}

void GameRenderer::render_action_info(const Action::Action_enum & selected_action)
{
  if ( selected_unit == nullptr ) { return; }

  std::array<bool, 30> row;
  row.fill(false);
  std::array<std::array<bool, 30>, 30> range;
  range.fill(row);
  for ( auto row : range )
  {
    row.fill(false);
  }

  if ( selected_unit->get_unit_type() == Unit::UnitClass::BOAT )
  {
    Boat* bt = (Boat*)selected_unit;
    KamikazeBoat * kb;
    Point p = bt->get_bow();
    switch ( selected_action )
    {
    case Action::Action_enum::FIRE_CANNON:
      range = bt->get_cannon_range();
      render_zone(range);
      break; // show range;
    case Action::Action_enum::LAY_MINE:
      range = bt->get_drop_pick_mine_range();
      render_zone(range);
      break; // show range;
    case Action::Action_enum::MOVE_FORWARDS:
      for ( int i = 0; i < bt->get_speed(); i++ )
      {
        p = p.get_point_in_direction(bt->get_position().get_direction(), 1);
        if ( !is_out_of_bounds(p) )
        {
          range[p.get_x()][p.get_y()] = true;
        }
      }
      render_zone(range);
      break; // show range;
    case Action::Action_enum::PICK_UP_MINE:
      range = bt->get_drop_pick_mine_range();
      render_zone(range);
      break; // show range;
    case Action::Action_enum::REPAIR:
      range = bt->get_repair_range();
      render_zone(range);
      break;
    case Action::Action_enum::KAMIKAZE_MOVE:
      kb = (KamikazeBoat*)bt;
      range = kb->get_move_range();
      render_zone(range);
      break;
    case Action::Action_enum::EXPLODE:
      kb = (KamikazeBoat*)bt;
      range = kb->get_move_range();
      render_zone(range);
      break;
    }
  }
  else if ( selected_unit->get_unit_type() == Unit::UnitClass::BASE )
  {
    Base* bs = (Base*)selected_unit;

    switch ( selected_action )
    {
    case Action::Action_enum::REPAIR:
      range = bs->get_repair_range();
      render_zone(range);
      break;
    }
  }
}

void GameRenderer::render_zone(std::array<std::array<bool, 30>, 30> range)
{
  for ( int i = 0; i < 30; i++ )
  {
    for ( int j = 0; j < 30; j++ )
    {
      if ( range[i][j] )
      {
        highlight_weapon_range.set_position_on_grid(i, j);
        highlight_weapon_range.render();
      }
    }
  }
}

void GameRenderer::set_selected_unit(const Unit * ut)
{
  selected_unit = ut;
}
const Unit * GameRenderer::get_selected_unit()
{
  return selected_unit;
}
void GameRenderer::reset_selected_unit()
{
  selected_unit = nullptr;
}

void GameRenderer::render_hidden(const GameState & gs, const Player::Side & side)
{
  for ( int i = 0; i < board.size(); i++ )
  {
    for ( int j = 0; j < board.size(); j++ )
    {
      if ( board[i][j] == Radar::Level::NONE )
      {
        fog.set_position_on_grid(i, j);
        fog.render();
      }
    }
  }
  for ( auto mn : gs.get_mines() )
  {
    Point p = mn.get_point();
    if ( board[p.get_x()][p.get_y()] == Radar::Level::SONAR )
    {
      mine.set_position_on_grid(p.get_x(), p.get_y());
      mine.render();
    }
  }
}
void GameRenderer::render_boat(const Boat & bt)
{
  boat_body.set_color(bt.get_color().red, bt.get_color().green, bt.get_color().blue);
  boat_head.set_color(bt.get_color().red, bt.get_color().green, bt.get_color().blue);
  Position pos = bt.get_position();
  Point cur = pos.get_point();
  Direction dir = pos.get_direction();
  int length = bt.get_length();
  for ( int i = 1; i <= length; i++ )
  {
    if ( i < length || length == 1 )
    {
      boat_body.set_position_on_grid(cur.get_x(), cur.get_y());
      boat_body.render();
      const Cell & cl = bt.get_cell_x_cells_away_from_back(i - 1);
      if ( cl.is_destroyed() )
      {
        body_destroyed.set_position_on_grid(cur.get_x(), cur.get_y());
        body_destroyed.render();
      }
      else if ( cl.is_damaged() )
      {
        body_damaged.set_position_on_grid(cur.get_x(), cur.get_y());
        body_damaged.render();
      }
      if ( ( length % 2 == 0 && length / 2 == i ) || ( length % 2 != 0 && length / 2 + 1 == i ) )
      {
        switch ( bt.get_boat_type() )
        {
        case Boat::BoatType::MINE_LAYER:
          diff_mine_layer.set_position_on_grid(cur.get_x(), cur.get_y());
          diff_mine_layer.render();
          break;
        case Boat::BoatType::RADAR_BOAT:
          diff_radar.set_position_on_grid(cur.get_x(), cur.get_y());
          diff_radar.render();
          break;
        case Boat::BoatType::TORPEDO_BOAT:
          diff_torpedo.set_position_on_grid(cur.get_x(), cur.get_y());
          diff_torpedo.render();
          break;
        case Boat::BoatType::REPAIR_BOAT:
          diff_repair.set_position_on_grid(cur.get_x(), cur.get_y());
          diff_repair.render();
          break;
        }
      }
    }
    else if ( i == length )
    {
      boat_head.set_position_on_grid(cur.get_x(), cur.get_y());
      double angle;
      switch ( dir )
      {
      case Direction::NORTH:
        angle = Angles::UP;
        break;
      case Direction::SOUTH:
        angle = Angles::DOWN;
        break;
      case Direction::WEST:
        angle = Angles::RIGHT;
        break;
      default:
        angle = Angles::LEFT;
        break;
      }
      boat_head.set_angle(angle);
      boat_head.render();
      const Cell & cl = bt.get_cell_x_cells_away_from_back(i - 1);
      if ( cl.is_destroyed() )
      {
        head_destroyed.set_position_on_grid(cur.get_x(), cur.get_y());
        head_destroyed.set_angle(angle);
        head_destroyed.render();
      }
      else if ( cl.is_damaged() )
      {
        head_damaged.set_position_on_grid(cur.get_x(), cur.get_y());
        head_damaged.set_angle(angle);
        head_damaged.render();
      }
    }
    if ( &bt == selected_unit )
    {
      highlight_selected_unit.set_position_on_grid(cur.get_x(), cur.get_y());
      highlight_selected_unit.render();
    }
    cur = cur.get_point_in_direction(dir, 1);
  }
  if ( bt.is_sunk() )
  {
    cur = pos.get_point();
    for ( int i = 0; i < length; i++ )
    {
      sunk.set_position_on_grid(cur.get_x(), cur.get_y());
      sunk.render();
      cur = cur.get_point_in_direction(dir, 1);
    }
  }
}
void GameRenderer::render_reef(const Reef & rf)
{
  Point p = rf.get_location();
  reef.set_position_on_grid(p.get_x(), p.get_y());
  reef.render();
}
void GameRenderer::render_base(const Base & bs)
{
  base.set_color(bs.get_color().red, bs.get_color().green, bs.get_color().blue);
  Position pos = bs.get_position();
  Point cur = pos.get_point();
  Direction dir = pos.get_direction();
  int length = bs.get_length();
  for ( int i = 0; i < length; i++ )
  {
    base.set_position_on_grid(cur.get_x(), cur.get_y());
    base.render();
    const Cell & cl = bs.get_cell_x_cells_away_from_back(i);
    if ( cl.is_destroyed() )
    {
      body_destroyed.set_position_on_grid(cur.get_x(), cur.get_y());
      body_destroyed.render();
    }
    else if ( cl.is_damaged() )
    {
      body_damaged.set_position_on_grid(cur.get_x(), cur.get_y());
      body_damaged.render();
    }
    if ( &bs == selected_unit )
    {
      highlight_selected_unit.set_position_on_grid(cur.get_x(), cur.get_y());
      highlight_selected_unit.render();
    }
    cur = cur.get_point_in_direction(dir, 1);
  }
  if ( bs.is_sunk() )
  {
    cur = pos.get_point();
    for ( int i = 0; i < length; i++ )
    {
      sunk.set_position_on_grid(cur.get_x(), cur.get_y());
      sunk.render();
      cur = cur.get_point_in_direction(dir, 1);
    }
  }
}

bool GameRenderer::is_unit_selected()
{
  return selected_unit != nullptr;
}

void GameRenderer::sort_units(std::vector<std::shared_ptr<Unit>> & units)
{
  std::sort(units.begin(), units.end(),
    [](std::shared_ptr<Unit> & a, std::shared_ptr<Unit> & b) -> bool
  {
    bool s1 = a->is_sunk();
    bool s2 = b->is_sunk();
    if ( ( s1 && s2 ) || s1 )
    {
      return true;
    }
    else
    {
      return false;
    }
  });
}
