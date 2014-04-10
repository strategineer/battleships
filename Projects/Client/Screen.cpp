#include "Screen.h"

Screen::Screen(const int tiles_wide, const int tiles_high)
{
  width_in_tiles = tiles_wide;
  height_in_tiles = tiles_high;
  tile_size = 25;
}
Screen::~Screen() {}
SDL_Point Screen::get_pixel_location_from_grid_location(const int row, const int column) const
{
  int i = row * tile_size;
  int j = column * tile_size;
  return SDL_Point{ i, j };
}
SDL_Point Screen::get_grid_location_from_pixel_location(const int x, const int y) const
{
  int row = x / tile_size;
  int column = y / tile_size;
  return SDL_Point{ row, column };
}
int Screen::get_width_in_pixels() const
{
  return width_in_tiles * tile_size;
}

int Screen::get_height_in_pixels() const
{
  return height_in_tiles * tile_size;
}

int Screen::get_width_in_tiles() const
{
  return width_in_tiles;
}
int Screen::get_height_in_tiles() const
{
  return height_in_tiles;
}
int Screen::get_tile_size() const
{
  return tile_size;
}
