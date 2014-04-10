#pragma once
#include <cstddef>
#include "Directions.h"
#include "Serializable.h"

class Point : public Serializable
{
public:
  Point();
  Point(const int pX, const int pY);
  Point(const Point p, const int d_x, const int d_y);

  Point get_point_in_direction(const Direction dir, const int count) const;
  Point get_north_by(const int count) const;
  Point get_south_by(const int count) const;
  Point get_east_by(const int count) const;
  Point get_west_by(const int count) const;

  bool north_of(const Point& o) const;
  bool south_of(const Point& o) const;
  bool east_of(const Point& o) const;
  bool west_of(const Point& o) const;
  bool operator==( const Point& other ) const;
  bool operator!=( const Point& other ) const;
  static int manhattan_distance(const Point& a, const Point& b);
  std::size_t get_hash() const;
  int get_x() const;
  int get_y() const;
  virtual JSONObject serialize() const;
  static Point unserialize(JSONObject object);
private:
  int x;
  int y;
};
