#pragma once
#include "Scene.h"
#include "Texture.h"
#include "Button.h"
#include "MessageSender.h"
/* Scene where the player can decide to find an opponent, change his/her settings or quit the game. */
class MainMenuScene :
  public Scene
{
public:
  MainMenuScene(MessageSender *ms);
  virtual ~MainMenuScene() override;
private:
  virtual void do_handle_event(SDL_Event& e) override;
  virtual void do_render() override;
  virtual void do_update() override;
  virtual void do_on_enter() override;
  virtual void do_on_exit() override;
  Texture title;
  Button find_match, create_match, quit;
  MessageSender *message_sender;
};

