#pragma once
#include "Command.h"
#include "SDL.h"
#include "Player.h"
#include "Reef.h"
#include "GameState.h"
#include "Enums.h"
/* Interface for SDL_Event handlers. */
class EventHandler
{
public:
  virtual ~EventHandler() = default;
  /* Reacts to the given event. */
  void handle_event(SDL_Event& e) { do_handle_event(e); }
  // For delivering commands from an external source into a particular scene.
  // A subclass that does not handle commands merely needs to leave the method
  // blank, but may optionally log to stderr.
  void deliver_command(const Command& c)
  {
    do_deliver_command(c);
  };

  void deliver_chat_message(std::string from, std::string msg)
  {
    do_deliver_chat_message(from, msg);
  };

  void deliver_notify_other_player_joined_game(std::string description)
  {
    do_deliver_notify_other_player_joined_game(description);
  };

  void deliver_new_reefs(std::vector<Reef> reefs)
  {
    do_deliver_new_reefs(reefs);
  }
  void deliver_reef_phase_over()
  {
    do_deliver_reef_phase_over();
  }
  void deliver_ship_setup_phase_over()
  {
    do_deliver_ship_setup_phase_over();
  }
  void deliver_other_player_quit_game()
  {
    do_deliver_other_player_quit_game();
  }
  void deliver_match_loaded(Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp)
  {
    do_deliver_match_loaded(turn, user_type, g, mp);
  }
  virtual void deliver_has_match_ended(Player::Side winner)
  {
    do_deliver_has_match_ended(winner);
  }



private:
  virtual void do_handle_event(SDL_Event& e) { return; }
  virtual void do_deliver_command(const Command& c) { return;  }
  virtual void do_deliver_chat_message(std::string from, std::string msg) { return; }
  virtual void do_deliver_notify_other_player_joined_game(std::string description) { return; }
  virtual void do_deliver_new_reefs(std::vector<Reef> reefs) { return; }
  virtual void do_deliver_reef_phase_over() { return; }
  virtual void do_deliver_ship_setup_phase_over() { return; }
  virtual void do_deliver_other_player_quit_game() { return; }
  virtual void do_deliver_match_loaded(Player::Side turn, Player::Side user_type, std::shared_ptr<GameState> g, Enums::MatchPhase mp) { return; }
  virtual void do_deliver_has_match_ended(Player::Side winner) { return; }
};

