#include "LoginScene.h"
#include "Fonts.h"
#include "Colors.h"
#include "MainMenuScene.h"
#include "Stage.h"
#include "BattleshipsRCP.h"

LoginScene::LoginScene(MessageSender *ms) :
message_sender(ms)
{
  entering_username = false;
  entering_password = false;
  stop_entering_username = false;
  stop_entering_password = false;
  username.set_texture(std::unique_ptr<Texture>(new Texture("Username:", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  username.set_position_by_screen_factor(0.25, 0.5);
  password.set_texture(std::unique_ptr<Texture>(new Texture("Password:", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  password.set_position_by_screen_factor(0.25, 0.65);
  create_account.set_texture(std::unique_ptr<Texture>(new Texture("Create Account", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  create_account.set_position_by_screen_factor(0.5, 0.1);
  submit.set_texture(std::unique_ptr<Texture>(new Texture("Submit", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  submit.set_position_by_screen_factor(0.7, 0.9);
  cancel.set_texture(std::unique_ptr<Texture>(new Texture("Cancel", Fonts::get_instance()->MENU_FONT, Colors::WHITE)));
  cancel.set_position_by_screen_factor(0.3, 0.9);
}


LoginScene::~LoginScene()
{}

void LoginScene::do_handle_event(SDL_Event& e)
{
  username.handle_event(e);
  password.handle_event(e);
  submit.handle_event(e);
  cancel.handle_event(e);
  create_account.handle_event(e);

  if ( username.has_been_clicked() ||
    password.has_been_clicked() ||
    submit.has_been_clicked() ||
    cancel.has_been_clicked() ||
    create_account.has_been_clicked() )
  {
    if ( entering_password )
    {
      stop_entering_password = true;
    }
    if ( entering_username )
    {
      stop_entering_username = true;
    }
  }

  if ( stop_entering_username )
  {
    stop_entering_username = false;
    SDL_StopTextInput();
    username_input.set_color(255, 255, 255);
    entering_username = false;
  }
  if ( stop_entering_password )
  {
    stop_entering_password = false;
    SDL_StopTextInput();
    password_input.set_color(255, 255, 255);
    entering_password = false;
  }

  if ( entering_username && e.type == SDL_TEXTINPUT )
  {
    /* Add new text onto the end of our text */
    username_text += e.text.text;
    username_input.load_from_rendered_text(username_text, Fonts::get_instance()->MENU_FONT, Colors::WHITE);
    username_input.set_color(200, 200, 200);
    username_input.set_position_by_screen_factor(0.7, 0.5);
  }
  else if ( entering_password && e.type == SDL_TEXTINPUT )
  {
    /* Add new text onto the end of our text */
    password_text += e.text.text;
    for ( int i = 0; i < strlen(e.text.text); i++ )
    {
      password_text_for_display += '*';
    }
    password_input.load_from_rendered_text(password_text_for_display, Fonts::get_instance()->MENU_FONT, Colors::WHITE);
    password_input.set_color(200, 200, 200);
    password_input.set_position_by_screen_factor(0.7, 0.6);
  }
  else if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE )
  {
    if ( entering_username )
    {
      int new_size = username_text.size() - 1;
      username_text = username_text.substr(0, std::max(0, new_size));
      username_input.load_from_rendered_text(username_text, Fonts::get_instance()->MENU_FONT, Colors::WHITE);
      username_input.set_color(200, 200, 200);
      username_input.set_position_by_screen_factor(0.7, 0.5);
    }
    else if ( entering_password )
    {
      int new_size = password_text.size() - 1;
      password_text = password_text.substr(0, std::max(0, new_size));
      password_text_for_display = password_text_for_display.substr(0, std::max(0, new_size));
      password_input.load_from_rendered_text(password_text_for_display, Fonts::get_instance()->MENU_FONT, Colors::WHITE);
      password_input.set_color(200, 200, 200);
      password_input.set_position_by_screen_factor(0.7, 0.6);
    }
  }
  else if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN )
  {
    if ( entering_username || entering_password )
    {
      try_login();
    }
  }
  else if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB )
  {
    if ( entering_password )
    {
      password_input.set_color(255, 255, 255);
      entering_password = false;
      entering_username = true;
    }
    else if ( entering_username )
    {
      username_input.set_color(255, 255, 255);
      entering_username = false;
      entering_password = true;
    }
  }
  else
  {
    if ( submit.has_been_clicked() )
    {
      try_login();
    }
    else if ( cancel.has_been_clicked() )
    {
      Stage::get_instance()->quit_game();
    }
    else if ( create_account.has_been_clicked() )
    {
      BattleshipsRCP rcp(message_sender);
      if ( rcp.register_user(username_text, password_text) )
      {
        Stage::get_instance()->notify("Account created!", 3000);
        Stage::get_instance()->push_scene(std::unique_ptr<Scene>(new MainMenuScene(message_sender)));
      }
      else
      {
        Stage::get_instance()->notify("Username already taken! Please try again.", 3000);
      }
    }
    else if ( e.type == SDL_QUIT )
    {
      Stage::get_instance()->quit_game();
    }
  }

  if ( password.has_been_clicked() )
  {
    if ( !entering_password )
    {
      entering_password = true;
      SDL_StartTextInput();
      password_text = "";
      password_text_for_display = "";
    }
  }

  if ( username.has_been_clicked() )
  {
    if ( !entering_username )
    {
      entering_username = true;
      SDL_StartTextInput();
      username_text = "";
    }
  }
}
void LoginScene::do_update()
{

}
void LoginScene::do_render()
{
  username.render();
  username_input.render();
  password.render();
  password_input.render();
  submit.render();
  cancel.render();
  create_account.render();
}
void LoginScene::do_on_enter()
{}
void LoginScene::do_on_exit()
{}

void LoginScene::try_login()
{
  BattleshipsRCP rcp(message_sender);
  if ( rcp.login_user(username_text, password_text) )
  {
    Stage::get_instance()->notify("Login successful!", 3000);
    Stage::get_instance()->push_scene(std::unique_ptr<Scene>(new MainMenuScene(message_sender)));
  }
  else
  {
    Stage::get_instance()->notify("Login error! Please try again.", 3000);
  }
}