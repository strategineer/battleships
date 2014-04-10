#include "Texture.h"
#include "TextureManager.h"

Texture::Texture()
{
  //Initialize
  texture = NULL;
  width = 0;
  height = 0;
  position = { 0, 0 };
  angle = 0.0;
  clip = NULL;
  center = NULL;
  flip = SDL_FLIP_NONE;
}

Texture::Texture(const std::string path) : Texture()
{
  load_from_file(path);
}
Texture::Texture(const std::string textureText, std::unique_ptr<Font> const &font, const SDL_Color textColor) : Texture()
{
  load_from_rendered_text(textureText, font, textColor);
}

Texture::~Texture()
{
  //Deallocate
  free();
}

bool Texture::load_from_file(const std::string path)
{
  //Get rid of preexisting texture
  free();

  //The final texture
  SDL_Texture* newTexture = NULL;

  std::string newPath = "images/" + path;

  //Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load(newPath.c_str());
  if ( loadedSurface == NULL )
  {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
  }
  else
  {
    //Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(TextureManager::get_instance()->get_renderer()->get_sdl_renderer(), loadedSurface);
    if ( newTexture == NULL )
    {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    }
    else
    {
      //Get image dimensions
      width = loadedSurface->w;
      height = loadedSurface->h;
    }

    //Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  //Return success
  texture = newTexture;
  return texture != NULL;
}

bool Texture::load_from_rendered_text(const std::string textureText, std::unique_ptr<Font> const &font, const SDL_Color textColor)
{
  //Get rid of preexisting texture
  free();

  //Render text surface
  SDL_Surface* textSurface = TTF_RenderText_Solid(font->get_sdl_font(), textureText.c_str(), textColor);
  if ( textSurface == NULL )
  {
    printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
  }
  else
  {
    //Create texture from surface pixels
    texture = SDL_CreateTextureFromSurface(TextureManager::get_instance()->get_renderer()->get_sdl_renderer(), textSurface);
    if ( texture == NULL )
    {
      printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      //Get image dimensions
      width = textSurface->w;
      height = textSurface->h;
    }

    //Get rid of old surface
    SDL_FreeSurface(textSurface);
  }

  //Return success
  return texture != NULL;
}

void Texture::free()
{
  //Free texture if it exists
  if ( texture != NULL )
  {
    SDL_DestroyTexture(texture);
    texture = NULL;
    width = 0;
    height = 0;
  }
}

void Texture::set_color(const Uint8 red, const Uint8 green, const Uint8 blue)
{
  //Modulate texture rgb
  SDL_SetTextureColorMod(texture, red, green, blue);
}

void Texture::set_blend_mode(const SDL_BlendMode blending)
{
  //Set blending function
  SDL_SetTextureBlendMode(texture, blending);
}

void Texture::set_alpha(const Uint8 alpha)
{
  //Modulate texture alpha
  SDL_SetTextureAlphaMod(texture, alpha);
}
void Texture::set_position(int x, int y)
{
  position.x = x - get_width() / 2;
  position.y = y - get_height() / 2;
}
void Texture::set_position_by_screen_factor(double width_factor, double height_factor)
{
  set_position(TextureManager::get_instance()->get_screen()->get_width_in_pixels() * width_factor,
    TextureManager::get_instance()->get_screen()->get_height_in_pixels() * height_factor);
}
void Texture::set_position_on_grid(int row, int col)
{
  position = TextureManager::get_instance()->get_screen()->get_pixel_location_from_grid_location(row, col);
}
void Texture::shift_position(int x_offset, int y_offset)
{
  position.x += x_offset;
  position.y += y_offset;
}
void Texture::set_angle(double ang)
{
  angle = ang;
}
void Texture::set_clip(SDL_Rect* cl)
{
  clip = cl;
}
void Texture::set_center(SDL_Point* cent)
{
  center = cent;
}
void Texture::set_flip(SDL_RendererFlip fl)
{
  flip = fl;
}

void Texture::do_render()
{
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { position.x, position.y, width, height };

  //Set clip rendering dimensions
  if ( clip != NULL )
  {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopyEx(TextureManager::get_instance()->get_renderer()->get_sdl_renderer(), texture, clip, &renderQuad, angle, center, flip);
}

int Texture::get_width() const
{
  return width;
}

int Texture::get_height() const
{
  return height;
}

SDL_Point Texture::get_position()
{
  return position;
}

int Texture::get_angle() const
{
  return angle;
}