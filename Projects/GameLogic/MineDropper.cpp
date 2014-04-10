#include "MineDropper.h"

int MineDropper::default_mine_damage = 2;

MineDropper::MineDropper(int pmine_damage, int pnum_of_mines, IMineHandler* pmine_handler)
: Weapon(pmine_damage, 1, 1, 1, 1), num_of_mines(pnum_of_mines), mine_handler(pmine_handler)
{

}

MineDropper::~MineDropper()
{

}

bool MineDropper::pick_up_mine(Point pick_up)
{
  if ( mine_handler->remove_mine(pick_up) )
  {
    num_of_mines++;
    return true;
  }
  return false;
}

bool MineDropper::drop_mine(Point drop_at)
{
  if ( num_of_mines > 0 && mine_handler->add_mine(Mine(drop_at, get_damage())) )
  {
    num_of_mines--;
    return true;
  }
  return false;
}

int MineDropper::get_number_of_mines_left() const
{
  return num_of_mines;
}

bool MineDropper::has_mines_left() const
{
  return num_of_mines > 0;
}

MineDropper MineDropper::get_default_mine_dropper(int pnum_of_mines, IMineHandler* pmine_handler)
{
  return MineDropper(default_mine_damage, 5, pmine_handler);
}

std::vector<Point> MineDropper::get_range(Position position, int unit_size) const
{
  Point unit_back = position.get_point();
  Direction unit_dir = position.get_direction();
  Point unit_front = position.get_point().get_point_in_direction(unit_dir, unit_size - 1);
  int row_length = unit_size;

  std::vector<Point> range_matrix;
  range_matrix.push_back(unit_back.get_point_in_direction(get_opposite_direction(unit_dir), 1));
  range_matrix.push_back(unit_front.get_point_in_direction(unit_dir, 1));
  Point current_point = unit_back;
  for ( int i = unit_size; i > 0; i-- )
  {
    range_matrix.push_back(current_point.get_point_in_direction(get_direction_after_turn(unit_dir, RIGHT), 1));
    range_matrix.push_back(current_point.get_point_in_direction(get_direction_after_turn(unit_dir, LEFT), 1));
    current_point = current_point.get_point_in_direction(unit_dir, 1);
  }
  return range_matrix;

}

JSONObject MineDropper::serialize() const
{
  JSONObject mine_dropper_serialize = Weapon::serialize();
  mine_dropper_serialize.set_value("name", JSONElement_ptr(new JSONString("MineDropper")));
  mine_dropper_serialize.set_value("num_of_mines", JSONElement_ptr(new JSONString(std::to_string(num_of_mines))));
  return mine_dropper_serialize;
}

MineDropper MineDropper::unserialize(JSONObject object, IMineHandler* pmine_handler)
{
  int new_damage = atoi(JSONString::extract_string(object["damage"]).c_str());
  int new_num_mines = atoi(JSONString::extract_string(object["num_of_mines"]).c_str());
  return MineDropper(new_damage, new_num_mines, pmine_handler);
}
