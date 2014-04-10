#include "utilities.h"
#include <cmath>
#include "Directions.h"

//Returns sign of a integer value
int utilities::sgn(double value)
{
  if ( value < 0.0 )
  {
    return -1;
  }
  else if ( value > 0.0 )
  {
    return 1;
  }
  return 0;
}

//Rounds a double at the 2nd decimal
double utilities::round(double d)
{
  return floor(d * 100 + 0.5) / 100;
};

//Returns cos (2nd decimal precision), takes as parameter angle in degrees
double utilities::cos(double degrees)
{
  return round(std::cos(degrees * PI / 180));
};

//Returns sin (2nd decimal precision), takes as parameter angle in degrees
double utilities::sin(double degrees)
{
  return round(std::sin(degrees * PI / 180));
};

//Checks if the Point is not out of bounds
bool is_out_of_bounds(Point point)
{
  if ( ( point.get_x() >= 0 && point.get_y() >= 0 ) && ( point.get_x() < 30 && point.get_y() < 30 ) )
  {
    return false;
  }
  return true;
};


//Get all points from starting point in the specified direction and distance
std::vector<Point> get_points_from(Position pos, int distance)
{
  std::vector<Point> points;
  for ( int i = 0; i <= distance; i++ )
  {
    Point location = get_point_at(pos, i);
    points.push_back(location);
  }
  return points;
};

//Gets the Point representing the location, at a distance in a specified direction from a specified starting position
Point get_point_at(Position pos, int distance)
{
  Point point = pos.get_point();
  Direction dir = pos.get_direction();
  int start_x = point.get_x();
  int start_y = point.get_y();
  if ( dir == NORTH )
  {
    point = Point(start_x, start_y - distance);
  }
  else if ( dir == SOUTH )
  {
    point = Point(start_x, start_y + distance);
  }
  else if ( dir == WEST )
  {
    point = Point(start_x - distance, start_y);
  }
  else if ( dir == EAST )
  {
    point = Point(start_x + distance, start_y);
  }
  return point;
};


std::vector<Point> get_points_in_range(Position position, int unit_size, int front, int back, int left, int right)
{
  Point unit_back = position.get_point();
  Direction unit_dir = position.get_direction();
  std::vector<Point> unit_points = get_points_from(position, unit_size - 1);

  Point starting_point = get_point_at(Position(unit_back, get_opposite_direction(unit_dir)), back);
  starting_point = get_point_at(Position(starting_point, get_direction_after_turn(unit_dir, LEFT)), left);

  int row_size = unit_size + back + front;
  std::vector<Point> range_matrix;
  for ( int i = 0; i < right + left + 1; i++ )
  {
    Point row_begin_point = get_point_at(Position(starting_point, get_direction_after_turn(unit_dir, RIGHT)), i);
    std::vector<Point> row_points = get_points_from(Position(row_begin_point, unit_dir), row_size - 1);
    for ( auto it = row_points.begin(); it != row_points.end(); it++ )
    {
      if ( std::find(unit_points.begin(), unit_points.end(), ( *it )) == unit_points.end() && !is_out_of_bounds(*it) )
      {
        range_matrix.push_back(*it);
      }
    }
  }
  return range_matrix;
}