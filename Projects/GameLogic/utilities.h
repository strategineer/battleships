#pragma once
#include <vector>
#include "Directions.h"
#include "Position.h"

const double PI = 3.14159265359;

namespace utilities
{
  //Returns sign of a integer value
  int sgn(double value);

  //Rounds a double at the 2nd decimal
  double round(double d);

  //Returns cos (2nd decimal precision), takes as parameter angle in degrees
  double cos(double degrees);

  //Returns sin (2nd decimal precision), takes as parameter angle in degrees
  double sin(double degrees);
}


std::vector<Point> get_points_from(Position pos, int length);
Point get_point_at(Position pos, int distance);
bool is_out_of_bounds(Point point);
std::vector<Point> get_points_in_range(Position position, int unit_size, int front, int back, int left, int right);