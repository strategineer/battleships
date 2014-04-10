#include "FindMatchesScene.h"
#include "MatchScene.h"
#include "Fonts.h"
#include "Colors.h"
#include "Stage.h"
#include "BattleshipsRCP.h"


FindMatchesScene::FindMatchesScene(MessageSender *ms) :
message_sender(ms), game_cursor(0), num_games_shown(5)
{
  back.set_texture(std::unique_ptr<Texture>(new Texture("Back", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  back.set_position_by_screen_factor(0.2, 0.9);
  refresh.set_texture(std::unique_ptr<Texture>(new Texture("Refresh", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  refresh.set_position_by_screen_factor(0.75, 0.9);
}
FindMatchesScene::~FindMatchesScene()
{}

void FindMatchesScene::do_handle_event(SDL_Event& e)
{

  for ( auto & b : match_buttons )
  {
    b->handle_event(e);
  }
  back.handle_event(e);
  refresh.handle_event(e);
  for ( auto & b : match_buttons )
  {
    if ( b->has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      JoinedGame joined_game = rcp.join_game(b->get_game_id());
      if ( !joined_game.error )
      {
        Stage::get_instance()->push_scene(std::unique_ptr<Scene>(new MatchScene(message_sender, joined_game.turn_of, joined_game.side, joined_game.state, Enums::MatchPhase::REEF_GENERATION)));

      }
      else
      {
        Stage::get_instance()->notify("That match does not exist anymore, please choose another.", 5000);
        refresh_list();
      }
    }
  }
  if ( back.has_been_clicked() )
  {
    Stage::get_instance()->pop_scene();
  }
  else if ( refresh.has_been_clicked() )
  {
    refresh_list();
    game_cursor = 0;
  }
  if ( e.type == SDL_KEYDOWN )
  {
    switch ( e.key.keysym.scancode )
    {
      case SDL_SCANCODE_DOWN:
        game_cursor = game_cursor + num_games_shown < match_buttons.size() ? game_cursor + num_games_shown : game_cursor;
        do_render();
        break;
      case SDL_SCANCODE_UP:
        game_cursor = game_cursor - num_games_shown >= 0 ? game_cursor - num_games_shown : game_cursor;
        do_render();
        break;
    }
  }
}

void FindMatchesScene::do_render()
{
  int i = 0;
  for ( auto & b : match_buttons )
  {
    if ( i >= game_cursor )
    {
      b->render();
    }
    i++;
    if ( i >= game_cursor + num_games_shown )
    {
      break;
    }
  }
  back.render();
  refresh.render();
}
void FindMatchesScene::do_update() {}
void FindMatchesScene::do_on_enter() 
{
  refresh_list();
  Stage::get_instance()->notify("to navigate!", 4000, Colors::RED);
  Stage::get_instance()->notify("UP or DOWN arrow key", 4000, Colors::RED);
}
void FindMatchesScene::do_on_exit() {}

void FindMatchesScene::refresh_list() 
{
  match_buttons.clear();
  BattleshipsRCP rcp(message_sender);
  auto games = rcp.get_current_games();

  for ( auto& g : games )
  {
    match_buttons.push_back(std::shared_ptr<MatchButton>(new MatchButton(g.first, g.second)));
  }
  double offset = 1.0 / match_buttons.size();
  double curr = 0.1;
  for ( auto& b : match_buttons )
  {
    b->set_position_by_screen_factor(0.5, curr);
    curr += offset;
    if ( curr >= num_games_shown * offset + 0.1 )
    {
      curr = 0.1;
    }
  }
}
