#pragma once
#include "Boat.h"

class KamikazeBoat : public Boat
{
private:
  int kamikaze_damage;
protected:
  KamikazeBoat(unsigned long pid, std::vector<Cell> pcells, const Position& p_location,
    ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler);
public:
  KamikazeBoat(const Position& initial_pos, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed);
  ~KamikazeBoat();
  static std::shared_ptr<Unit> s_construct_kamikaze_boat(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);

  virtual CommandResult move(Point target, bool kamikaze);
  std::array<std::array<bool, 30>, 30> get_move_range() const;
  bool is_in_range(Point target) const;

  virtual JSONObject serialize() const;
  static KamikazeBoat unserialize(JSONObject object, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler);
  virtual CommandResult perform_action(const Command& cmd) override;
  bool check_if_path_exists(Point target);
};