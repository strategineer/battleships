#include "Point.h"
#include <unordered_set>
#include <sstream>

Point::Point() :
Serializable(),
x(0),
y(0)
{};

Point::Point(const int pX, const int pY) :
x(pX),
y(pY)
{};

Point::Point(const Point p, const int d_x, const int d_y)
{
  x = p.x + d_x;
  y = p.y + d_y;
};

int Point::get_x() const
{
  return x;
};

int Point::get_y() const
{
  return y;
};

Point Point::get_point_in_direction(const Direction dir, const int count) const
{
  switch ( dir )
  {
  case Direction::NORTH:
    return get_north_by(count);
    break;
  case Direction::EAST:
    return get_east_by(count);
    break;
  case Direction::WEST:
    return get_west_by(count);
    break;
  default:
    return get_south_by(count);
    break;
  }
}

Point Point::get_north_by(const int count) const
{
  return Point(x, ( y - count ));
}

Point Point::get_south_by(const int count) const
{
  return Point(x, ( y + count ));
}

Point Point::get_east_by(const int count) const
{
  return Point(( x + count ), y);
}

Point Point::get_west_by(const int count) const
{
  return Point(( x - count ), y);
}

bool Point::north_of(const Point& o) const
{
  return ( o.y >= y );
}

bool Point::south_of(const Point& o) const
{
  return ( o.y <= y );
}

bool Point::east_of(const Point& o) const
{
  return ( o.x <= x );
}

bool Point::west_of(const Point& o) const
{
  return ( o.x >= x );
}

/* Returns the Manhattan distance between two points, the sum of the absolute
 * distances between their x-values and y-values.
 */
int Point::manhattan_distance(const Point& a, const Point& b)
{
  int x_dist = a.x - b.x;
  int y_dist = a.y - b.y;
  x_dist = ( x_dist > 0 ) ? x_dist : -x_dist;
  y_dist = ( y_dist > 0 ) ? y_dist : -y_dist;
  return x_dist + y_dist;
};

bool Point::operator==( const Point& other ) const
{
  if ( other.get_x() == x && other.get_y() == y )
  {
    return true;
  }
  return false;
};


bool Point::operator!=( const Point& other ) const
{
  if ( other.get_x() != x || other.get_y() != y )
  {
    return true;
  }
  return false;
};

//Get hash value of a tuple
std::size_t Point::get_hash() const
{
  int hash = 7;
  hash = 71 * hash + ( *this ).x;
  hash = 71 * hash + ( *this ).y;
  return hash;
}

JSONObject Point::serialize() const
{
  JSONObject point_json = JSONObject::get_template("Class", "Point");
  point_json.set_value("x", JSONElement_ptr(new JSONString(std::to_string(x))));
  point_json.set_value("y", JSONElement_ptr(new JSONString(std::to_string(y))));
  return point_json;
}

Point Point::unserialize(JSONObject object)
{
  int new_x = atoi(JSONString::extract_string(object["x"]).c_str());
  int new_y = atoi(JSONString::extract_string(object["y"]).c_str());
  return Point(new_x, new_y);
}
