#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <memory>
#include <cmath>
#include "Screen.h"
#include "Angles.h"
#include "Font.h"
#include "Renderable.h"
/* Wrapper of an SDL_Texture* providing several useful convenience methods. */
class Texture
  : public Renderable
{
public:
  Texture();
  /* Constructor which loads a texture from an image. */
  Texture(const std::string path);
  /* Constructor which loads a texture from text. */
  Texture(const std::string textureText, std::unique_ptr<Font> const &font, const SDL_Color textColor);
  ~Texture();
  /* Load the texture from the specified path to an image. */
  bool load_from_file(const std::string path);
  /* Load a texture, given some text, a font and a color. */
  bool load_from_rendered_text(const std::string textureText, std::unique_ptr<Font> const &font, const SDL_Color textColor);
  /* Set the color modulation. */
  void set_color(const Uint8 red, const Uint8 green, const Uint8 blue);
  /* Set the blending mode. */
  void set_blend_mode(const SDL_BlendMode blending);
  /* Set the alpha modulation. */
  void set_alpha(const Uint8 alpha);
  /* Set the texture's position, by center. */
  void set_position(int x, int y);
  /* Set the texture's position using a factor of the screen's width and height. */
  void set_position_by_screen_factor(double width_factor, double height_factor);
  /* Set the textuer's position using the grid, useful for tiles. */
  void set_position_on_grid(int row, int col);
  /* Shifts the texture's position by the given x and y offsets. */
  void shift_position(int x_offset, int y_offset);
  /* Set the texture's angle. */
  void set_angle(double ang);
  /* Set the texture's clipping rectangle. */
  void set_clip(SDL_Rect* cl);
  /* Set the texture's center, for rotation purposes. */
  void set_center(SDL_Point* cent);
  /* Set the texture's renderer flip. */
  void set_flip(SDL_RendererFlip fl);

  //Gets image dimensions
  int get_width() const;
  int get_height() const;
  SDL_Point get_position();
  int get_angle() const;

private:
  /* Deallocates the texture. */
  void free();
  virtual void do_render() override;
  /* The actual hardware texture. */
  SDL_Texture* texture;
  SDL_Point position;
  double angle;
  SDL_Rect* clip;
  SDL_Point* center;
  SDL_RendererFlip flip;

  //Image dimensions
  int width;
  int height;
};

