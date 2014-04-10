#pragma once
#include "JSONObject.h"
#include "Command.h"

class ClientMessageMaker
{
public:
  static JSONObject make_command_message(Command& command);
  static JSONObject make_chat_message(std::string chat_message);
  static JSONObject make_reef_response_message(bool accept_reef);
  static JSONObject make_save_game_message(std::string save_game_name);
  static JSONObject make_load_game_message(int save_game_id);
  static JSONObject make_join_game_message(int game_id);
  static JSONObject make_create_game_message();
  static JSONObject make_get_save_game_message();
  static JSONObject make_get_all_games_message();
  static JSONObject make_register_user_message(std::string user, std::string password);
  static JSONObject make_login_user_message(std::string user, std::string password);
  static JSONObject make_prep_phase_done_message();
  static JSONObject make_quit_message();

};