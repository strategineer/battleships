#pragma once
#include <string>
#include "Button.h"
#include "Texture.h"
#include "Action.h"

class OverlayElement
{
public:
  OverlayElement();
  OverlayElement(std::unique_ptr<Texture> txt, const Action::Action_enum & act);
  OverlayElement(const std::string & str, const Action::Action_enum & act);
  ~OverlayElement();
  void set_position(int x, int y);
  Button& get_button();
  const Action::Action_enum & get_action() const;
private:
  Action::Action_enum action;
  std::shared_ptr<Button> button;
};

