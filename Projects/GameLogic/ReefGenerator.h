#pragma once
#include <vector>
#include "Point.h"

class ReefGenerator
{
public:
  ReefGenerator();
  ~ReefGenerator();
  static std::vector<Point> generate_reef(int width_in_tiles, int height_in_tiles, double probability);
};