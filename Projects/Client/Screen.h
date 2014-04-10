#pragma once
#include "SDL.h"
#include <memory>
#include <vector>
/* Class representing the Battleships GUI window. */
class Screen
{
public:
  Screen(const int tiles_wide, const int tiles_high);
  ~Screen();
  /* Given a row and a column, returns the top left pixel of the specified tile. */
  SDL_Point get_pixel_location_from_grid_location(const int row, const int column) const;
  /* 
    Given an x and y, finds the specified tile on the grid and returns it as
    its row index and column index.
  */
  SDL_Point get_grid_location_from_pixel_location(const int x, const int y) const;
  int get_width_in_pixels() const;
  int get_height_in_pixels() const;
  int get_width_in_tiles() const;
  int get_height_in_tiles() const;
  int get_tile_size() const;
private:
  int width_in_tiles;
  int height_in_tiles;
  int tile_size;
};

