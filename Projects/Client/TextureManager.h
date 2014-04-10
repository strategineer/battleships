#pragma once
#include <memory>
#include "SDL.h"
#include "Screen.h"
#include "Renderer.h"
/* Singleton class used by Texture(s) for rendering and positioning purposes. */
class TextureManager
{
public:
  ~TextureManager();
  static std::unique_ptr<TextureManager> const & get_instance();
  static void set_instance(std::unique_ptr<Renderer> const &rend, std::unique_ptr<Screen> const &scr);
  std::unique_ptr<Renderer> const & get_renderer();
  std::unique_ptr<Screen> const & get_screen();
private:
  TextureManager(std::unique_ptr<Renderer> const &rend, std::unique_ptr<Screen> const &scr);
  static std::unique_ptr<TextureManager> instance;
  std::unique_ptr<Renderer> const & renderer;
  std::unique_ptr<Screen> const & screen;
};

