#pragma once
#include "SDL.h"
#include "Base.h"
#include "Boat.h"
#include "Reef.h"
#include "Texture.h"
#include "Mine.h"
#include "GameState.h"
#include "Overlay.h"
#include "Action.h"
class GameRenderer
{
public:
  GameRenderer();
  ~GameRenderer();
  void render_game_state(const GameState & gs, const Player::Side & side, const Action::Action_enum & selected_action);
  void set_selected_unit(const Unit * ut);
  const Unit * get_selected_unit();
  bool is_unit_selected();
  void reset_selected_unit();
  void render_action_info(const Action::Action_enum & selected_action);
private:
  void render_hidden(const GameState & gs, const Player::Side & side);
  void render_background();
  void render_boat(const Boat & bt);
  void render_reef(const Reef & rf);
  void render_base(const Base & bs);
  void render_zone(std::array<std::array<bool, 30>, 30> range);
  void sort_units(std::vector<std::shared_ptr<Unit>> & units);
  const Unit * selected_unit;
  std::array<std::array<Radar::Level, 30>, 30> board;
  Texture ocean_dark, ocean_light, boat_body, boat_head, reef, base, mine, fog;
  Texture body_damaged, body_destroyed, head_damaged, head_destroyed;
  Texture diff_radar, diff_torpedo, diff_repair, diff_mine_layer;
  Texture sunk;
  Texture highlight_selected_unit, highlight_weapon_range, highlight_mine_pickup, highlight_radar_range;
};

