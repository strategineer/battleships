#pragma once
#include "Renderable.h"
#include "Unit.h"
#include "InfoTexture.h"
class UnitInformation : public Renderable
{
public:
  UnitInformation();
  ~UnitInformation();
  void popup_at(int x, int y);
  void set_selected_unit(const Unit * ut);
  void reset_selected_unit();
private:
  virtual void do_render() override;
  const Unit * selected_unit;
  InfoTexture name, speed, is_armored;
};

