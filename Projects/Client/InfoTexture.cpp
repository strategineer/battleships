#include "InfoTexture.h"
#include "Fonts.h"
#include "Colors.h"

InfoTexture::InfoTexture()
{}

InfoTexture::InfoTexture(std::string pre) : prefix(pre)
{}


InfoTexture::~InfoTexture()
{}

void InfoTexture::set_prefix(std::string pre)
{
  prefix = pre;
  update_texture();
}
void InfoTexture::set_value(std::string val)
{
  value = val;
  update_texture();
}
void InfoTexture::update_texture()
{
  std::string text = prefix + value;
  if ( text.empty() ) { text = "-"; }
  load_from_rendered_text(prefix + value, Fonts::get_instance()->MESSAGE_FONT, Colors::WHITE);
}
