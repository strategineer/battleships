#pragma once
#include "Scene.h"
#include "NotificationManager.h"
#include <stack>
#include <queue>
#include <memory>
#include <utility>
#include "Screen.h"
#include "SceneAction.h"
#include "Point.h"
#include "Player.h"
#include "Enums.h"
#include "GameState.h"
/* Class manages the rendering, event handling and transisitions between Scene(s).*/
class Stage
  : public Renderable, public EventHandler
{
public:
  static std::unique_ptr<Stage> const & get_instance();
  static void set_instance(std::unique_ptr<Screen> const &scr);
  ~Stage();
  /* Shows the player the given message for the specified time. */
  void notify(std::string msg, int time_in_ms);
  void notify(std::string msg, int time_in_ms, const SDL_Color color);
  /* Shows the player the given ping for the specified time. */
  void ping(const Point & p, int time_in_ms);
  /* Calls update on the current scene. */
  void update();
  /* Returns true if the stage has one or more scenes on the stack. */
  bool has_scene();
  /* Pops all the scenes from the stack. */
  void quit_game();
  /* Push the given scene onto the stack. */
  void push_scene(std::unique_ptr<Scene> &scn);
  /* Pops the current scene off the stack and pushes the given scene onto it. */
  void switch_to_scene(std::unique_ptr<Scene> &scn);
  /* Pops the current scene off the stack. */
  void pop_scene();
  std::unique_ptr<Screen> const& get_screen();
private:
  Stage(std::unique_ptr<Screen> const &scr);
  static std::unique_ptr<Stage> instance;
  void do_render();
  void do_handle_event(SDL_Event& e);
  virtual void do_deliver_command(const Command& c) override;
  virtual void do_deliver_chat_message(std::string from, std::string msg) override;
  virtual void do_deliver_notify_other_player_joined_game(std::string description) override;
  virtual void do_deliver_new_reefs(std::vector<Reef> reefs) override;
  virtual void do_deliver_reef_phase_over() override;
  virtual void do_deliver_ship_setup_phase_over() override;
  virtual void do_deliver_other_player_quit_game() override;
  virtual void do_deliver_match_loaded(Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp) override;
  virtual void do_deliver_has_match_ended(Player::Side winner) override;
  /* Pops all the scenes from the stack. */
  void internal_quit_game();
  /* Push the given scene onto the stack. */
  void internal_push_scene(std::unique_ptr<Scene> &scn);
  /* Pops the current scene off the stack and pushes the given scene onto it. */
  void internal_switch_to_scene(std::unique_ptr<Scene> &scn);
  /* Pops the current scene off the stack. */
  void internal_pop_scene();
  std::unique_ptr<Screen> const &screen;
  std::unique_ptr<NotificationManager> notification_manager;
  /* Stack of scenes, the scene on top of the stack is the current scene. */
  std::stack<std::unique_ptr<Scene>> scenes;
  std::queue<std::unique_ptr<SceneAction>> events;
};

