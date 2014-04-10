#include "Radar.h"


Radar::Radar()
: left(3), right(3), front(3), back(3), radar_level(Level::RADAR)

{}

Radar::Radar(int l, int r, int f, int b, Level lvl)
: left(l), right(r), front(f), back(b), radar_level(lvl)
{}

Radar::~Radar()
{}

void Radar::set_range(int l, int r, int f, int b)
{
  left = l;
  right = r;
  front = f;
  back = b;
}
void Radar::set_level(Level lvl)
{
  radar_level = lvl;
}

void Radar::add_visibility(Position boat_position, std::array<std::array<Level, 30>, 30> & board) const
{
  int west, east, north, south;

  //convert to absolute coordinates
  switch ( boat_position.get_direction() )
  {
  case Direction::NORTH:
    west = left;
    east = right;
    north = front;
    south = back;
    break;
  case Direction::SOUTH:
    west = right;
    east = left;
    north = back;
    south = front;
    break;
  case Direction::WEST:
    west = front;
    east = back;
    north = right;
    south = left;
    break;
  case Direction::EAST:
    west = back;
    east = front;
    north = left;
    south = right;
    break;
  }
  //get lower-left/south-west point and upper-right/north-east point
  //shift these points if out of bounds
  int x, y;
  x = boat_position.get_point().get_x() - west;
  x = x >= 0 ? x : 0;
  y = boat_position.get_point().get_y() - north;
  y = y >= 0 ? y : 0;
  Point north_west(x, y);
  x = boat_position.get_point().get_x() + east;
  x = x < 30 ? x : 29;
  y = boat_position.get_point().get_y() + south;
  y = y < 30 ? y : 29;
  Point south_east(x, y);

  for ( int i = north_west.get_x(); i <= south_east.get_x(); i++ )
  {
    for ( int j = north_west.get_y(); j <= south_east.get_y(); j++ )
    {
      if ( board[i][j] < radar_level )
      {
        board[i][j] = radar_level;
      }
    }
  }
  //make back point of boat visible even if not covered by radar
  board[boat_position.get_point().get_x()][boat_position.get_point().get_y()] = radar_level;
}


JSONObject Radar::serialize() const
{
  JSONObject radar_serialize = JSONObject::get_template("Class", "Radar");

  radar_serialize.set_value("left", JSONElement_ptr(new JSONString(std::to_string(left))));
  radar_serialize.set_value("right", JSONElement_ptr(new JSONString(std::to_string(right))));
  radar_serialize.set_value("front", JSONElement_ptr(new JSONString(std::to_string(front))));
  radar_serialize.set_value("back", JSONElement_ptr(new JSONString(std::to_string(back))));
  radar_serialize.set_value("radar_level", JSONElement_ptr(new JSONString(std::to_string(radar_level))));

  return radar_serialize;
}

Radar Radar::unserialize(JSONObject object)
{
  int new_right = atoi(JSONString::extract_string(object["right"]).c_str());
  int new_left = atoi(JSONString::extract_string(object["left"]).c_str());
  int new_front = atoi(JSONString::extract_string(object["front"]).c_str());
  int new_back = atoi(JSONString::extract_string(object["back"]).c_str());
  Level new_radar_level = static_cast<Level>( atoi(JSONString::extract_string(object["radar_level"]).c_str()) );
  return Radar::Radar(new_left, new_right, new_front, new_back, new_radar_level);
}