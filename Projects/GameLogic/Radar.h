#pragma once
#include <array>
#include "Position.h"

class Radar : public Serializable
{
public:
  enum Level
  {
    NONE,
    RADAR,
    SONAR
  };
  Radar::Radar();
  /* loc center of radar
     dir direction of boat
     l, r, f, b, radar range for each side
     lvl 1 for normal radar, 2 for sonar
     */
  Radar(int l, int r, int f, int b, Level lvl);
  ~Radar();
  /* given int[][30], sets visible grid locations to radar_level if current < radar_level */
  void add_visibility(Position boat_position, std::array<std::array<Level, 30>, 30> & board) const;
  void set_range(int l, int r, int f, int b);
  void set_level(Level lvl);
  virtual JSONObject serialize() const;
  static Radar unserialize(JSONObject object);
private:
  int left, right, front, back;
  Level radar_level;
};

