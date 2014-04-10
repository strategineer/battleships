#pragma once
#include <memory>
#include "Scene.h"
#include "Screen.h"
#include "Texture.h"
#include "Button.h"
#include "MessageSender.h"
/* Scene where the player logs in to an existing account or creates a new one. */
class LoginScene :
  public Scene
{
public:
  LoginScene(MessageSender *ms);
  virtual ~LoginScene() override;
private:
  virtual void do_handle_event(SDL_Event& e) override;
  virtual void do_render() override;
  virtual void do_update() override;
  virtual void do_on_enter() override;
  virtual void do_on_exit() override;
  void try_login();
  Button username, password;
  bool entering_username, entering_password, stop_entering_username, stop_entering_password;
  std::string username_text, password_text, password_text_for_display;
  Texture username_input, password_input;
  Button create_account, submit, cancel;
  MessageSender *message_sender;
};

