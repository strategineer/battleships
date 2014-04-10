#pragma once
#include "Directions.h"
#include "Unit.h"

class Base : public Unit
{
public:
  Base();
  Base(Position initial_position, int health, int b_size,
    ICollisionDetector *p_collision_detector);

  virtual CommandResult perform_action(const Command& cmd) override;

  virtual Unit::UnitClass get_unit_type() const override;

  std::array<std::array<bool, 30>, 30> get_repair_range() const;
  virtual CommandResult repair_boat(Point target);

  static std::shared_ptr<Unit> s_construct_base(const Position& p, const RgbColor& col,
    ICollisionDetector *collision_detector);

  virtual JSONObject serialize() const;
  static Base unserialize(JSONObject object, ICollisionDetector *collision_detector);

protected:
  Base(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
    ICollisionDetector *p_collision_detector);

};
