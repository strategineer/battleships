#pragma once
#include <memory>
#include "ServerUser.h"
#include "GameState.h"
#include <mutex>
#include "GameInfo.h"
#include "LoadedGame.h"

class CurrentGame
{
private:
  std::mutex game_mutex;
  std::shared_ptr<GameState> state;
  std::shared_ptr<ServerUser> player1;
  std::shared_ptr<ServerUser> player2;
  std::vector<std::shared_ptr<ServerUser>> spectators;
  Player::Side turn_of;
  int game_id;
public:
  enum ReefDecision
  {
    UNDECIDED, ACCEPTED, REFUSED
  };
  CurrentGame();
  CurrentGame(int id, std::shared_ptr<GameState>& game_state);
  ~CurrentGame();
  void load_game(LoadedGame game);
  std::shared_ptr<GameState> get_game_state();
  Player::Side add_user(std::shared_ptr<ServerUser> user);
  void apply_to_state(std::shared_ptr<ServerUser> user, JSONObject object);
  void send_all_except_me(std::shared_ptr<ServerUser> me, std::string message);
  void send_all(std::string message);
  ReefDecision add_user_decision(std::string username, bool accepted);
  GameInfo get_game_info();
  std::string get_player_one();
  std::string get_player_two();
  std::pair<bool, Player::Side> game_is_won() const;
  void lock_game();
  void unlock_game();
  Player::Side get_turn_of();
  int get_game_id();
  std::vector<std::string> get_spectators();
  bool player_is_ready(std::string username);
  void reset_reef_decision();
  bool is_player(std::string username);
  void remove_spectator(std::string username);
  void toggle_turn();
private:
  ReefDecision player1_decision;
  ReefDecision player2_decision;
  bool player1_ready_to_start;
  bool player2_ready_to_start;
};