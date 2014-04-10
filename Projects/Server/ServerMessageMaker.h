#pragma once
#include "JSON.h"
#include "Command.h"
#include "GameState.h"
#include "GameInfo.h"
#include "SavedGame.h"

class ServerMessageMaker
{
public:
  //response messages
  static JSONObject make_register_response_message(bool response);
  static JSONObject make_login_response_message(bool response);
  static JSONObject make_save_game_response_message(bool response);
  static JSONObject make_create_game_response_message(std::shared_ptr<GameState> game_state);
  static JSONObject make_join_game_response_message(bool error, Player::Side turn_of, Player::Side side, std::shared_ptr<GameState> game_state);
  static JSONObject make_get_save_games_response_message(std::vector<SavedGame> saved_games);
  static JSONObject make_load_game_response_message(int turn_of, Player::Side side, std::shared_ptr<GameState> game_state);
  static JSONObject make_get_current_games_response_message(std::vector<GameInfo> current_games);

  //notify messages
  static JSONObject make_notify_reef_config_message(bool accept, std::vector<Reef> reefs);
  static JSONObject make_notify_prep_phase_done_message();
  static JSONObject make_notify_player_joined_game_message(std::string username, int wins, int losses);
  static JSONObject make_notify_end_game_message(Player::Side winner);
  static JSONObject make_notify_quit_game_message();
  static JSONObject make_notify_load_game_message(Player::Side turn_of, Player::Side side, std::shared_ptr<GameState> game_state);

};