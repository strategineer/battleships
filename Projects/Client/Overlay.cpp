#include "Overlay.h"
#include <memory>

Overlay::Overlay()
{
  selected_unit = nullptr;
  action_result_valid = false;

  auto txt_move_forwards = std::unique_ptr<Texture>(new Texture("action/move.png"));
  txt_move_forwards->set_angle(Angles::UP);
  move_forwards = OverlayElement(std::move(txt_move_forwards), Action::Action_enum::MOVE_FORWARDS);
  elements.push_back(&move_forwards);
  auto txt_move_backwards = std::unique_ptr<Texture>(new Texture("action/move.png"));
  txt_move_backwards->set_angle(Angles::DOWN);
  move_backwards = OverlayElement(std::move(txt_move_backwards), Action::Action_enum::MOVE_BACKWARDS);
  elements.push_back(&move_backwards);
  auto txt_turn_left = std::unique_ptr<Texture>(new Texture("action/move_rotate_left.png"));
  turn_left = OverlayElement(std::move(txt_turn_left), Action::Action_enum::TURN_LEFT);
  elements.push_back(&turn_left);
  auto txt_turn_right = std::unique_ptr<Texture>(new Texture("action/move_rotate_right.png"));
  turn_right = OverlayElement(std::move(txt_turn_right), Action::Action_enum::TURN_RIGHT);
  elements.push_back(&turn_right);
  auto txt_turn_back = std::unique_ptr<Texture>(new Texture("action/move_rotate_right.png"));
  txt_turn_back->set_angle(Angles::DOWN);
  turn_back = OverlayElement(std::move(txt_turn_back), Action::Action_enum::TURN_BACK);
  elements.push_back(&turn_back);
  auto txt_slide_left = std::unique_ptr<Texture>(new Texture("action/move_shift.png"));
  txt_slide_left->set_angle(Angles::RIGHT);
  slide_left = OverlayElement(std::move(txt_slide_left), Action::Action_enum::SLIDE_LEFT);
  elements.push_back(&slide_left);
  auto txt_slide_right = std::unique_ptr<Texture>(new Texture("action/move_shift.png"));
  txt_slide_right->set_angle(Angles::LEFT);
  slide_right = OverlayElement(std::move(txt_slide_right), Action::Action_enum::SLIDE_RIGHT);
  elements.push_back(&slide_right);
  fire_cannon = OverlayElement("action/cannon.png", Action::Action_enum::FIRE_CANNON);
  elements.push_back(&fire_cannon);
  fire_torpedo = OverlayElement("action/torpedo.png", Action::Action_enum::FIRE_TORPEDO);
  elements.push_back(&fire_torpedo);
  toggle_secondary_radar = OverlayElement("action/deploy_radar.png", Action::Action_enum::TOGGLE_SECONDARY_RADAR);
  elements.push_back(&toggle_secondary_radar);
  lay_mine = OverlayElement("action/mine_drop.png", Action::Action_enum::LAY_MINE);
  elements.push_back(&lay_mine);
  pickup_mine = OverlayElement("action/mine_pickup.png", Action::Action_enum::PICK_UP_MINE);
  elements.push_back(&pickup_mine);
  repair = OverlayElement("action/repair.png", Action::Action_enum::REPAIR);
  elements.push_back(&repair);
  move_kami = OverlayElement("action/move_kami.png", Action::Action_enum::KAMIKAZE_MOVE);
  elements.push_back(&move_kami);
  suicide = OverlayElement("action/suicide.png", Action::Action_enum::EXPLODE);
  elements.push_back(&suicide);
  reset_selected_unit();
}

Overlay::~Overlay()
{}

void Overlay::set_selected_unit(const Unit * ut)
{
  unit_info.set_selected_unit(ut);
  selected_unit = ut;
  reset_selected_unit();
  if ( selected_unit != nullptr )
  {
    for ( auto e : elements )
    {
      for ( auto cap = selected_unit->get_capabilities_head(); cap != selected_unit->get_capabilities_tail(); cap++ )
      {
        if ( e->get_action() == *cap )
        {
          e->get_button().enable();
          break;
        }
      }
    }
  }
}

void Overlay::reset_selected_unit()
{
  unit_info.reset_selected_unit();
  for ( auto e : elements )
  {
    e->get_button().disable();
  }
}
void Overlay::popup_at(int x, int y)
{
  unit_info.popup_at(x, y + 100);
  int off = 25;
  move_kami.set_position(x, y);
  move_forwards.set_position(x, y - off);
  move_backwards.set_position(x, y + off);
  turn_left.set_position(x - off, y);
  turn_right.set_position(x + off, y);
  turn_back.set_position(x, y + off * 2);
  slide_left.set_position(x - off * 2, y);
  slide_right.set_position(x + off * 2, y);
  fire_cannon.set_position(x + off, y - off);
  fire_torpedo.set_position(x + off * 2, y - off);
  suicide.set_position(x + off * 3, y - off);
  lay_mine.set_position(x - off, y - off);
  pickup_mine.set_position(x - off * 2, y - off);
  toggle_secondary_radar.set_position(x + off, y + off);
  repair.set_position(x - off, y + off);
}
void Overlay::do_render()
{
  for ( auto & ele : elements )
  {
    ele->get_button().render();
  }
  unit_info.render();
}
void Overlay::do_handle_event(SDL_Event& e)
{
  action_result_valid = false;
  for ( auto & ele : elements )
  {
    ele->get_button().handle_event(e);
  }

  for ( auto & ele : elements )
  {
    if ( ele->get_button().has_been_clicked() )
    {
      handle_event_action_result = ele->get_action();
      action_result_valid = true;
    }
  }
}

Action::Action_enum Overlay::get_handle_event_action_result() const
{
  return handle_event_action_result;
}

bool Overlay::is_action_result_valid() const
{
  return action_result_valid;
}