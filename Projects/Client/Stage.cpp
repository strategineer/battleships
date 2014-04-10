#include "Stage.h"
#include "Colors.h"
std::unique_ptr<Stage> Stage::instance = nullptr;

Stage::Stage(std::unique_ptr<Screen> const &scr) : screen(scr)
{
  notification_manager = std::unique_ptr<NotificationManager>(new NotificationManager());
}
Stage::~Stage()
{}

std::unique_ptr<Stage> const & Stage::get_instance()
{
  return instance;
}

void Stage::set_instance(std::unique_ptr<Screen> const &scr)
{
  instance = std::unique_ptr<Stage>(new Stage(scr));
}

void Stage::notify(std::string msg, int time_in_ms)
{
  notification_manager->display_notification(msg, time_in_ms);
}

void Stage::notify(std::string msg, int time_in_ms, const SDL_Color color)
{
    notification_manager->display_notification(msg, time_in_ms, color);
}

void Stage::ping(const Point & p, int time_in_ms)
{
  notification_manager->display_ping(p, time_in_ms);
}

void Stage::do_handle_event(SDL_Event& e)
{
  if ( has_scene() )
  {
    scenes.top()->handle_event(e);
  }
}
void Stage::update()
{
  while ( !events.empty() )
  {
    switch ( events.front()->event )
    {
    case SceneAction::SceneEvent::Pop:
      internal_pop_scene(); break;
    case SceneAction::SceneEvent::Push:
      internal_push_scene(std::move(events.front()->scene)); break;
    case SceneAction::SceneEvent::Switch:
      internal_switch_to_scene(std::move(events.front()->scene)); break;
    case SceneAction::SceneEvent::Quit:
      internal_quit_game(); break;
    }
    events.pop();
  }
  if ( has_scene() )
  {
    scenes.top()->update();
    notification_manager->update();
  }
}
void Stage::do_render()
{
  if ( has_scene() )
  {
    scenes.top()->render();
    notification_manager->render();
  }
}
void Stage::quit_game()
{
  events.push(std::unique_ptr<SceneAction>(new SceneAction(SceneAction::SceneEvent::Quit, std::unique_ptr<Scene>())));
}
void Stage::push_scene(std::unique_ptr<Scene> &scn)
{
  events.push(std::unique_ptr<SceneAction>(new SceneAction(SceneAction::SceneEvent::Push, std::move(scn))));
}
void Stage::switch_to_scene(std::unique_ptr<Scene> &scn)
{
  events.push(std::unique_ptr<SceneAction>(new SceneAction(SceneAction::SceneEvent::Switch, std::move(scn))));
}
void Stage::pop_scene()
{
  events.push(std::unique_ptr<SceneAction>(new SceneAction(SceneAction::SceneEvent::Pop, std::unique_ptr<Scene>())));
}
void Stage::internal_quit_game()
{
  while ( !scenes.empty() )
  {
    scenes.pop();
  }
}
void Stage::internal_push_scene(std::unique_ptr<Scene> &scn)
{
  if ( has_scene() ) 
  {
    scenes.top()->on_exit();
  }
  scenes.push(std::move(scn));
  scenes.top()->on_enter();
}
void Stage::internal_switch_to_scene(std::unique_ptr<Scene> &scn)
{
  if ( has_scene() )
  {
    scenes.top()->on_exit();
    scenes.pop();
  }
  scenes.push(std::move(scn));
  scenes.top()->on_enter();
}
void Stage::internal_pop_scene()
{
  if ( has_scene() )
  {
    scenes.top()->on_exit();
    scenes.pop();
  }
  if ( has_scene() )
  {
    scenes.top()->on_enter();
  }
}
bool Stage::has_scene()
{
  return !scenes.empty();
}

std::unique_ptr<Screen> const& Stage::get_screen()
{
  return screen;
}

void Stage::do_deliver_command(const Command& c)
{
  if ( has_scene() )
  {
    scenes.top()->deliver_command(c);
  }
}

void Stage::do_deliver_chat_message(std::string from, std::string msg)
{
  notify(from + ": " + msg, 5000, Colors::BLACK);
}

void Stage::do_deliver_notify_other_player_joined_game(std::string description)
{
  if ( has_scene() )
  {
    scenes.top()->deliver_notify_other_player_joined_game(description);
  }
}
void Stage::do_deliver_new_reefs(std::vector<Reef> reefs)
{
  if ( has_scene() )
  {
    scenes.top()->deliver_new_reefs(reefs);
  }
}
void Stage::do_deliver_reef_phase_over()
{
  if ( has_scene() )
  {
    scenes.top()->deliver_reef_phase_over();
  }
}
void Stage::do_deliver_ship_setup_phase_over()
{
  if ( has_scene() )
  {
    scenes.top()->deliver_ship_setup_phase_over();
  }
}
void Stage::do_deliver_other_player_quit_game()
{
  if ( has_scene() )
  {
    scenes.top()->deliver_other_player_quit_game();
  }
}
void Stage::do_deliver_match_loaded(Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp)
{
  if ( has_scene() )
  {
    scenes.top()->deliver_match_loaded(turn, user_type, g, mp);
  }
}
void Stage::do_deliver_has_match_ended(Player::Side winner)
{
  if ( has_scene() )
  {
    scenes.top()->deliver_has_match_ended(winner);
  }
}