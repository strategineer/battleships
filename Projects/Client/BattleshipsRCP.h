#pragma once
#include "MessageSender.h"
#include <vector>
#include <memory>
#include "GameState.h"

struct JoinedGame
{
  bool error;
  Player::Side side;
  Player::Side turn_of;
  std::shared_ptr<GameState> state;
};

class BattleshipsRCP
{
private:
  MessageSender *message_sender;
public:
  BattleshipsRCP(MessageSender *ms);
  ~BattleshipsRCP();

  bool register_user(std::string user, std::string pass);
  bool login_user(std::string user, std::string pass);
  bool save_game(std::string name);
  JoinedGame join_game(int game_id);
  void send_chat(std::string chat_message);
  
  //int: save_game_id
  std::vector<std::pair<int, std::string>> get_saved_games();
  
   
  void load_game(int save_game_id);

  //int: game_id
  std::vector<std::pair<int, std::string>> get_current_games();

  void submit_reef_configuration_decision(bool decision);
  void submit_command(Command& command);

  void submit_prep_phase_done();
  std::shared_ptr<GameState> create_game();
  void quit_game();

};
