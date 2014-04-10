#pragma once
#include "Scene.h"
#include <memory>
#include "Button.h"
#include "MessageSender.h"
#include "MatchButton.h"
/* Scene where the player can send, receive and accept invitations to a lobby/room to play a match. */
class FindMatchesScene :
  public Scene
{
public:
  FindMatchesScene(MessageSender *ms);
  ~FindMatchesScene();
private:
  virtual void do_handle_event(SDL_Event& e) override;
  virtual void do_render() override;
  virtual void do_update() override;
  virtual void do_on_enter() override;
  virtual void do_on_exit() override;
  void refresh_list();
  Button back, refresh;
  std::vector<std::shared_ptr<MatchButton>> match_buttons;
  MessageSender *message_sender;
  int game_cursor;
  int num_games_shown;
};

