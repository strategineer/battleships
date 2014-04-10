#include "OverlayElement.h"

OverlayElement::OverlayElement() :button(std::shared_ptr<Button>(new Button()))
{

}

OverlayElement::OverlayElement(std::unique_ptr<Texture> txt, const Action::Action_enum & act) : action(act), button(std::shared_ptr<Button>(new Button()))
{
  ( *button ).set_texture(std::move(txt));
}

OverlayElement::OverlayElement(const std::string & str, const Action::Action_enum & act) :
OverlayElement(std::unique_ptr<Texture>(new Texture(str)), act)
{

}

OverlayElement::~OverlayElement()
{}

void OverlayElement::set_position(int x, int y)
{
  ( *button ).set_position(x, y);
}
Button & OverlayElement::get_button()
{
  return ( *button );
}
const Action::Action_enum & OverlayElement::get_action() const
{
  return action;
}
