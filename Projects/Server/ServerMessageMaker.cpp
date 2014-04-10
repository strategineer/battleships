#include "ServerMessageMaker.h"
#include "ClientServerMessages.h"

JSONObject ServerMessageMaker::make_register_response_message(bool response)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::REGISTER_RESPONSE)));
  std::string value = response ? ServerMessages::SUCCESS : ServerMessages::FAILURE;
  message.set_value("message_value", JSONElement_ptr(new JSONString(value)));
  return message;
}

JSONObject ServerMessageMaker::make_login_response_message(bool response)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::LOGIN_RESPONSE)));
  std::string value = response ? ServerMessages::SUCCESS : ServerMessages::FAILURE;
  message.set_value("message_value", JSONElement_ptr(new JSONString(value)));
  return message;
}

JSONObject ServerMessageMaker::make_save_game_response_message(bool response)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::SAVE_GAME_RESPONSE)));
  std::string value = response ? ServerMessages::SUCCESS : ServerMessages::FAILURE;
  message.set_value("message_value", JSONElement_ptr(new JSONString(value)));
  return message;
}

JSONObject ServerMessageMaker::make_join_game_response_message(bool error, Player::Side turn_of, Player::Side side, std::shared_ptr<GameState> game_state)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::JOIN_GAME_RESPONSE)));
  JSONObject value;
  value.set_value("error", JSONElement_ptr(new JSONString(std::to_string(error))));
  value.set_value("turn_of", JSONElement_ptr(new JSONString(std::to_string(static_cast<int>( turn_of )))));
  value.set_value("side", JSONElement_ptr(new JSONString(std::to_string(static_cast<int>( side )))));
  if ( game_state != nullptr )
  {
    value.set_value("game_state", JSONElement_ptr(new JSONObject(game_state->serialize())));
  }
  else
  {
    value.set_value("game_state", JSONElement_ptr(new JSONString("")));
  }
  message.set_value("message_value", JSONElement_ptr(new JSONObject(value)));
  return message;
}

JSONObject ServerMessageMaker::make_get_save_games_response_message(std::vector<SavedGame> saved_games)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::GET_SAVED_GAMES_RESPONSE)));
  JSONArray current_games_json_array;
  for ( auto it = saved_games.begin(); it != saved_games.end(); it++ )
  {
    JSONObject o;
    o.set_value("save_game_id", JSONElement_ptr(new JSONString(std::to_string(( *it ).saved_game_id))));
    o.set_value("name", JSONElement_ptr(new JSONString(( *it ).name)));
    o.set_value("timestamp", JSONElement_ptr(new JSONString(( *it ).timestamp)));
    current_games_json_array.add(JSONElement_ptr(new JSONObject(o)));
  }
  message.set_value("message_value", JSONElement_ptr(new JSONArray(current_games_json_array)));
  return message;
}

JSONObject ServerMessageMaker::make_load_game_response_message(int turn_of, Player::Side side, std::shared_ptr<GameState> game_state)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::LOAD_GAME_RESPONSE)));
  JSONObject value;
  value.set_value("turn_of", JSONElement_ptr(new JSONString(std::to_string(turn_of))));
  value.set_value("side", JSONElement_ptr(new JSONString(std::to_string(static_cast<int>(side)))));
  value.set_value("game_state", JSONElement_ptr(new JSONObject(game_state->serialize())));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(value)));
  return message;
}

JSONObject ServerMessageMaker::make_get_current_games_response_message(std::vector<GameInfo> current_games)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::GET_CURRENT_GAMES_RESPONSE)));
  JSONArray current_games_json_array;
  for ( auto it = current_games.begin(); it != current_games.end(); it++ )
  {
    JSONObject o;
    o.set_value("game_id", JSONElement_ptr(new JSONString(std::to_string(( *it ).game_id))));
    o.set_value("created_by", JSONElement_ptr(new JSONString(( *it ).created_by)));
    o.set_value("wins", JSONElement_ptr(new JSONString(std::to_string(( *it ).creator_wins))));
    o.set_value("losses", JSONElement_ptr(new JSONString(std::to_string(( *it ).creator_losses))));
    o.set_value("num_players_in_game", JSONElement_ptr(new JSONString(std::to_string(( *it ).num_players_in_game))));
    o.set_value("max_players_in_game", JSONElement_ptr(new JSONString(std::to_string(( *it ).max_players_in_game))));
    current_games_json_array.add(JSONElement_ptr(new JSONObject(o)));
  }
  message.set_value("message_value", JSONElement_ptr(new JSONArray(current_games_json_array)));
  return message;
}

JSONObject ServerMessageMaker::make_notify_reef_config_message(bool accept, std::vector<Reef> reefs)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::REEF_CONFIG_ANSWER)));
  std::string accept_string = accept ? ServerMessages::SUCCESS : ServerMessages::FAILURE;
  JSONObject inner_object;
  inner_object.set_value("accept_reef", JSONElement_ptr(new JSONString(accept_string)));
  JSONArray reef_json_array;
  for ( auto it = reefs.begin(); it != reefs.end(); it++ )
  {
    reef_json_array.add(JSONElement_ptr(new JSONObject(it->serialize())));
  }
  inner_object.set_value("reef_config", JSONElement_ptr(new JSONArray(reef_json_array)));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(inner_object)));
  return message;
}

JSONObject ServerMessageMaker::make_notify_prep_phase_done_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::PREP_PHASE_DONE)));
  return message;
}

JSONObject ServerMessageMaker::make_notify_player_joined_game_message(std::string username, int wins, int losses)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::NOTIFY_PLAYER_JOINED_GAME)));
  JSONObject o;
  o.set_value("username", JSONElement_ptr(new JSONString(username)));
  o.set_value("wins", JSONElement_ptr(new JSONString(std::to_string(wins))));
  o.set_value("losses", JSONElement_ptr(new JSONString(std::to_string(losses))));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(o)));
  return message;
}

JSONObject ServerMessageMaker::make_notify_end_game_message(Player::Side winner)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::NOTIFY_END_GAME)));
  message.set_value("message_value", JSONElement_ptr(new JSONString(std::to_string(winner))));
  return message;
}

JSONObject ServerMessageMaker::make_create_game_response_message(std::shared_ptr<GameState> game_state)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::CREATE_GAME_ANSWER)));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(game_state->serialize())));
  return message;
}

JSONObject ServerMessageMaker::make_notify_quit_game_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::NOTIFY_PLAYER_QUIT_GAME)));
  return message;
}

JSONObject ServerMessageMaker::make_notify_load_game_message(Player::Side turn_of, Player::Side side, std::shared_ptr<GameState> game_state)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ServerMessages::NOTIFY_GAME_LOADED)));
  JSONObject value;
  value.set_value("turn_of", JSONElement_ptr(new JSONString(std::to_string(static_cast<int>(turn_of)))));
  value.set_value("side", JSONElement_ptr(new JSONString(std::to_string(static_cast<int>(side)))));
  value.set_value("game_state", JSONElement_ptr(new JSONObject(game_state->serialize())));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(value)));
  return message;
}