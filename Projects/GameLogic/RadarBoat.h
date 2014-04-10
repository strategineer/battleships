#pragma once
#include "Boat.h"
#include "Radar.h"

class RadarBoat :
  public Boat
{
private:
  Radar deployed_radar;
  bool is_deployed_flag;
public:
  RadarBoat(const Position& initial_pos, int cell_health, int p_length,
    ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler,
    int p_speed, int p_rotation_speed, int p_center);
  ~RadarBoat();
  virtual CommandResult move_forwards(unsigned int distance) override;
  virtual CommandResult move_backwards(unsigned int distance) override;
  virtual CommandResult rotate(RelativeDirection dir, unsigned int rotation_units) override;
  virtual CommandResult shift(RelativeDirection dir);

  virtual CommandResult perform_action(const Command& cmd) override;
  void set_deployed_radar_range(int l, int r, int f, int b);
  void set_deployed_radar_level(Radar::Level lvl);
  void toggle_deployed();
  bool is_deployed();
  virtual void add_visibility(std::array<std::array<Radar::Level, 30>, 30> & board) const;

  static std::shared_ptr<Unit> s_construct_radar_boat(const Position & p, const RgbColor & col,
    ICollisionDetector* collision_detector, IMineHandler* pmine_handler);

  virtual JSONObject serialize() const;
  static RadarBoat unserialize(JSONObject object, ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler);

protected:
  RadarBoat(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
    ICollisionDetector *p_collision_detector, IMineHandler* pmine_handler, int p_speed,
    int p_rotation_speed, int p_center);

};
