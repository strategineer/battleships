#pragma once
#include "Texture.h"
class InfoTexture : public Texture
{
public:
  InfoTexture();
  InfoTexture(std::string pre);
  ~InfoTexture();
  void set_prefix(std::string pre);
  void set_value(std::string val);
private:
  void update_texture();
  std::string prefix, value;
};

