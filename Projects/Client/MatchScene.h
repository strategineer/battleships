#pragma once
#include "Scene.h"
#include <memory>
#include "MessageSender.h"
#include "Screen.h"
#include "Texture.h"
#include "Overlay.h"
#include "GameRenderer.h"
#include "Boat.h"
#include "Base.h"
#include "Reef.h"
#include "GameState.h"
#include "Action.h"
#include "MatchButton.h"
#include "Enums.h"
/* Scene where the player plays against another player over the network. */
class MatchScene :
  public Scene
{
public:
  MatchScene(MessageSender *ms);
  MatchScene(MessageSender *ms, Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp);
  virtual ~MatchScene() override;

private:
  virtual void do_handle_event(SDL_Event& e) override;
  virtual void do_deliver_command(const Command& c) override;
  virtual void do_deliver_notify_other_player_joined_game(std::string description) override;
  virtual void do_deliver_new_reefs(std::vector<Reef> reefs) override;
  virtual void do_deliver_reef_phase_over() override;
  virtual void do_deliver_ship_setup_phase_over() override;
  virtual void do_deliver_other_player_quit_game() override;
  virtual void do_deliver_match_loaded(Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp) override;
  virtual void do_deliver_has_match_ended(Player::Side winner) override;
  virtual void do_handle_command_result(const CommandResult& cr, bool is_enemy_notification);
  virtual void do_render() override;
  virtual void do_update() override;
  virtual void do_on_enter() override;
  virtual void do_on_exit() override;
  void handle_event_for_all(SDL_Event& e);
  void handle_event_for_spectator(SDL_Event& e);
  void handle_event_for_reef_generation(SDL_Event& e);
  void handle_event_for_ship_setup(SDL_Event& e);
  void handle_event_for_gameplay(SDL_Event& e);
  void toggle_turn();
  void toggle_side();
  void try_command(const Command & cmd);
  const Unit * get_selected_unit_by_click(const int x, const int y) const;
  void set_selected_unit(const Unit * ut);
  void reset_selected_unit();

  std::string chat_message;
  std::string save_game_name;
  Texture chat_texture;
  bool is_typing;

  Enums::MatchPhase currentPhase;
  Player::Side view_side, real_side, turn_of;
  std::shared_ptr<GameState> gs;
  GameRenderer drawer;
  Overlay action_overlay;
  Action::Action_enum selected_action;

  Button reef_accept, reef_reject;
  bool has_made_decision_new_reef;
  Button ship_setup_done;
  bool has_finished_setting_up_ships;
  Button quit, save_game, load_game;
  
  bool is_loading_match;
  bool is_game_over;
  MessageSender *message_sender;
  std::vector<std::shared_ptr<MatchButton>> saved_matches;
  const bool is_local_play;
  bool is_action_overlay_up, is_meta_menu_up;
  bool are_saved_matches_up;
  bool need_more_input;
  int saved_game_cursor;
  int num_saved_games_shown;
};

