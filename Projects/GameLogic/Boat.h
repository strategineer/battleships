#pragma once
#include <memory>
#include "Directions.h"
#include "Unit.h"
#include "Mine.h"
#include "Torpedo.h"
#include "Cannon.h"
#include "CommandResult.h"
#include "MineDropper.h"
class Boat : public Unit
{
public:
  enum BoatType
  {
    CRUISER,
    DESTROYER,
    MINE_LAYER,
    TORPEDO_BOAT,
    REPAIR_BOAT,
    RADAR_BOAT,
    KAMIKAZE_BOAT
  };
private:
  unsigned int speed;
  unsigned int rotation_speed;
  unsigned int center;
  BoatType boat_type;

public:
  Boat(const Position& initial_pos, int cell_health, int p_length,
    ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler,
    int p_speed, int p_rotation_speed, int p_center, BoatType ptype);

  virtual CommandResult move_forwards(Point target);
  virtual CommandResult move_forwards(unsigned int distance);
  virtual CommandResult move_backwards(unsigned int distance);
  virtual CommandResult shift(RelativeDirection dir);
  virtual CommandResult rotate(RelativeDirection dir, unsigned int rotation_units);

  virtual CommandResult perform_action(const Command& cmd) override;

  virtual Unit::UnitClass get_unit_type() const override;
  BoatType get_boat_type() const;
  static std::string convert_boat_type_to_string(BoatType ptype);
  Point get_rotation_point() const;
  int get_speed() const;

  static std::shared_ptr<Unit> s_construct_cruiser(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);
  static std::shared_ptr<Unit> s_construct_destroyer(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);
  // If MineLayer becomes a separate subclass of Boat this is a candidate for moving to that class.
  static std::shared_ptr<Unit> s_construct_mine_layer(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);
  static std::shared_ptr<Unit> s_construct_torpedo_boat(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);
  static std::shared_ptr<Unit> s_construct_repair_boat(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);

  virtual JSONObject serialize() const;
  static Boat unserialize(JSONObject object, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler);

  void init_cannon(int pright, int pleft, int pfront, int pback, Cannon::CannonType type);
  void init_mine_dropper(int num_of_mines);
  void init_torpedo();
  bool has_mines() const;
  bool has_mine_layer() const;
  int get_num_mines_left() const;

  std::array<std::array<bool, 30>, 30> get_torpedo_range() const;
  std::array<std::array<bool, 30>, 30> get_drop_pick_mine_range() const;
  std::array<std::array<bool, 30>, 30> get_cannon_range() const;
  std::array<std::array<bool, 30>, 30> get_repair_range() const;

  CommandResult fire_torpedo(int prand);
  CommandResult fire_cannon(Point target);
  CommandResult drop_mine(Point target);
  CommandResult pick_up_mine(Point pick_up_point);
  CommandResult repair_boat(Point target);

protected:
  Boat(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
    ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed,
    int p_rotation_speed, int p_center, BoatType ptype);
  virtual CommandResult handle_explosive_collision(Explosive* exp, Point collision_point, std::shared_ptr<ICollidable> colliding_object, int prand);
  virtual void handle_boat_collision(Point boat_collision_point, Point collision_point, std::shared_ptr<ICollidable> colliding_object, CommandResult& command_result);
  IMineHandler* mine_handler;
  std::shared_ptr<MineDropper> mine_dropper;
  std::shared_ptr<Torpedo> torpedo;
  std::shared_ptr<Cannon> cannon;

};
