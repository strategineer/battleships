#include "TextureManager.h"

std::unique_ptr<TextureManager> TextureManager::instance = nullptr;

TextureManager::TextureManager(std::unique_ptr<Renderer> const &rend, std::unique_ptr<Screen> const &scr) : renderer(rend), screen(scr)
{}
TextureManager::~TextureManager()
{}
std::unique_ptr<TextureManager> const & TextureManager::get_instance()
{
  return instance;
}
void TextureManager::set_instance(std::unique_ptr<Renderer> const &rend, std::unique_ptr<Screen> const &scr)
{
  instance = std::unique_ptr<TextureManager>(new TextureManager(rend, scr));
}
std::unique_ptr<Renderer> const & TextureManager::get_renderer()
{
  return renderer;
}

std::unique_ptr<Screen> const & TextureManager::get_screen()
{
  return screen;
}
