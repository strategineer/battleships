#pragma once
#include "SDL.h"
#include "Renderable.h"
#include "EventHandler.h"
#include <vector>
#include "OverlayElement.h"
#include "Action.h"
#include "Directions.h"
#include "Unit.h"
#include "UnitInformation.h"
/*
  Class responsible for displays actions available to the player
  depending on the selected ship and allows the player to pick one.
  */
class Overlay
  : public Renderable, public EventHandler
{
public:
  Overlay();
  ~Overlay();
  /* Set the Overlay's position. */
  void popup_at(int x, int y);
  void set_selected_unit(const Unit * ut);
  void reset_selected_unit();
  /* Return the result of the previous handle_event call. */
  Action::Action_enum get_handle_event_action_result() const;
  bool is_action_result_valid() const;
private:
  virtual void do_render() override;
  virtual void do_handle_event(SDL_Event& e) override;
  virtual void do_deliver_command(const Command& c) override
  {
    return;
  };
  std::vector<OverlayElement *> elements;
  const Unit * selected_unit;
  UnitInformation unit_info;
  OverlayElement move_forwards, move_backwards, turn_left,
    turn_right, turn_back, slide_left, slide_right, fire_cannon, fire_torpedo,
    lay_mine, pickup_mine, toggle_secondary_radar, repair, move_kami, suicide;
  Action::Action_enum handle_event_action_result;
  bool action_result_valid;
};

