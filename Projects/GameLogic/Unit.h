#pragma once
#include <iterator>
#include <vector>
#include "Action.h"
#include "Cell.h"
#include "Command.h"
#include "ICollisionDetector.h"
#include "ICollidable.h"
#include "Position.h"
#include "RgbColor.h"
#include "Radar.h"
#include "CommandResult.h"

class Unit : public ICollidable, Serializable
{

public:

  enum UnitClass
  {
    UNIT,
    BOAT,
    BASE
  };

  Unit(int health, int unit_length, const Position& p_location,
    ICollisionDetector *p_collision_detector);
  virtual ~Unit();
  bool contains_point(const Point& point) const;
  virtual bool heal_damage(int amount);
  void receive_damage(const Point& point, int amount);
  virtual bool will_collide(const Point& pt) const;
  virtual bool collide_at(const Point& pt, int damage);
  bool is_sunk() const;
  void set_position(Position pos);
  // This method's prototype will change once I figure out what it needs, i.e.
  // the union of all possible required values.  Each action will take specific
  // ones.

  virtual CommandResult perform_action(const Command& cmd);

  Point get_bow() const;

  const Position & get_position() const;
  const int get_length() const;
  Direction get_direction() const;
  int get_size() const;
  const unsigned long get_id() const;
  int get_maximum_health() const;

  /* Color Methods */
  const RgbColor & get_color() const;
  void set_color(const int r, const int g, const int b);
  void set_color(const RgbColor & col);
  /* Radar Methods*/
  virtual void add_visibility(std::array<std::array<Radar::Level, 30>, 30> & board) const;
  void set_radar_range(int l, int r, int f, int b);
  void set_radar_level(Radar::Level lvl);
  const Cell & get_cell_x_cells_away_from_back(int x) const;

  virtual UnitClass get_unit_type() const;

  // Returns an iterator over the capabilites of the unit. I recommend using the
  // `auto` keyword for this type in general.
  std::vector<const Action::Action_enum>::const_iterator get_capabilities_head() const;
  std::vector<const Action::Action_enum>::const_iterator get_capabilities_tail() const;

  virtual JSONObject serialize() const;
  static Unit unserialize(JSONObject object, ICollisionDetector *p_collision_detector);

  static bool is_unit(ICollidable* exp);
  std::vector<Point> get_points() const;
protected:
  std::vector<const Action::Action_enum> capabilities;
  std::vector<Cell>& access_cells();
  unsigned int length;
  Position location;
  ICollisionDetector *collision_detector;
  RgbColor color;
  Radar radar;
  UnitClass type;
  Unit(unsigned long pid, int maximum_health, std::vector<Cell> pcells, const Position& p_location,
    ICollisionDetector *p_collision_detector);
  void destroy_unit();
private:
  static unsigned long s_id;
  unsigned long id;
  int maximum_health;
  std::vector<Cell> cells;

};

