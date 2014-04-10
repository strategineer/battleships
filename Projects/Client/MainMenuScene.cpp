#include <memory>

#include "MainMenuScene.h"
#include "FindMatchesScene.h"
#include "MatchScene.h"
#include "Fonts.h"
#include "Colors.h"
#include "Angles.h"
#include "Stage.h"
#include "BattleshipsRCP.h"


MainMenuScene::MainMenuScene(MessageSender *ms) :
message_sender(ms)
{
  title.load_from_rendered_text("Battleships", Fonts::get_instance()->TITLE_FONT, Colors::WHITE);
  title.set_position_by_screen_factor(0.5, 0.2);
  find_match.set_texture(std::unique_ptr<Texture>(new Texture("Find Match", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  find_match.set_position_by_screen_factor(0.5, 0.4);
  create_match.set_texture(std::unique_ptr<Texture>(new Texture("Create Match", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  create_match.set_position_by_screen_factor(0.5, 0.6);
  quit.set_texture(std::unique_ptr<Texture>(new Texture("Quit", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  quit.set_position_by_screen_factor(0.5, 0.8);
}
MainMenuScene::~MainMenuScene() {}
void MainMenuScene::do_handle_event(SDL_Event& e)
{
  find_match.handle_event(e);
  create_match.handle_event(e);
  quit.handle_event(e);
  if ( find_match.has_been_clicked() )
  {
    Stage::get_instance()->push_scene(std::unique_ptr<Scene>(new FindMatchesScene(message_sender)));
  }
  else if ( create_match.has_been_clicked() )
  {
    BattleshipsRCP rcp(message_sender);
    //TODO: RCP create_game????
    std::shared_ptr<GameState> game_state = rcp.create_game();
    Stage::get_instance()->push_scene(std::unique_ptr<Scene>(new MatchScene(message_sender, Player::PLAYER_ONE, Player::PLAYER_ONE, game_state, Enums::MatchPhase::WAITING_FOR_OTHER_PLAYER)));
  }
  else if ( quit.has_been_clicked() )
  {
    Stage::get_instance()->quit_game();
  }
  //If user closes the window
  else if ( e.type == SDL_QUIT )
  {
    Stage::get_instance()->quit_game();
  }
}

void MainMenuScene::do_update()
{

}


void MainMenuScene::do_render()
{
  title.render();
  find_match.render();
  create_match.render();
  quit.render();
}

void MainMenuScene::do_on_enter()
{

}

void MainMenuScene::do_on_exit()
{

}