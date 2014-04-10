#pragma once
#include "Weapon.h"
#include "Mine.h"
#include "IMineHandler.h"

class MineDropper : public Weapon
{
private:
  static int default_mine_damage;
  int num_of_mines;
  IMineHandler* mine_handler;
public:
  MineDropper(int pmine_damage, int pnum_of_mines, IMineHandler* pmine_handler);
  ~MineDropper();
  bool pick_up_mine(Point pick_up);
  bool drop_mine(Point drop_at);
  int get_number_of_mines_left() const;
  bool has_mines_left() const;
  virtual std::vector<Point> get_range(Position position, int unit_size) const;
  virtual JSONObject serialize() const;
  static MineDropper unserialize(JSONObject object, IMineHandler* pmine_handler);
  static MineDropper get_default_mine_dropper(int pnum_of_mines, IMineHandler* pmine_handler);
};