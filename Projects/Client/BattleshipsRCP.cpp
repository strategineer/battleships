#include "BattleshipsRCP.h"
#include "ClientMessageMaker.h"
#include "ClientServerMessages.h"

BattleshipsRCP::BattleshipsRCP(MessageSender *ms) : message_sender(ms)
{

}

BattleshipsRCP::~BattleshipsRCP()
{

}

//blocking
bool BattleshipsRCP::register_user(std::string user, std::string pass)
{
  if ( user.empty() || pass.empty() )
  {
    return false;
  }
  JSONObject message = ClientMessageMaker::make_register_user_message(user, pass);
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::REGISTER_RESPONSE
      && JSONString::extract_string(object["message_value"]) == ServerMessages::SUCCESS )
  {
    return true;
  }
  return false;
}

//blocking
bool BattleshipsRCP::login_user(std::string user, std::string pass)
{
  if ( user.empty() || pass.empty() )
  {
    return false;
  }
  JSONObject message = ClientMessageMaker::make_login_user_message(user, pass);
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::LOGIN_RESPONSE
      && JSONString::extract_string(object["message_value"]) ==  ServerMessages::SUCCESS )
  {
    return true;
  }
  return false;
}

//blocking
bool BattleshipsRCP::save_game(std::string name)
{
  JSONObject message = ClientMessageMaker::make_save_game_message(name);
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::SAVE_GAME_RESPONSE
      && JSONString::extract_string(object["message_value"]) == ServerMessages::SUCCESS )
  {
    return true;
  }
  return false;
}

//blocking
JoinedGame BattleshipsRCP::join_game(int game_id)
{
  JoinedGame joined_game;
  JSONObject message = ClientMessageMaker::make_join_game_message(game_id);
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::JOIN_GAME_RESPONSE)
  {
    JSONObject value = dynamic_cast< JSONObject & >( *object["message_value"] );
    joined_game.error = ( bool ) atoi(JSONString::extract_string(value["error"]).c_str());
    joined_game.turn_of = ( Player::Side ) atoi(JSONString::extract_string(value["turn_of"]).c_str());
    joined_game.side = ( Player::Side ) atoi(JSONString::extract_string(value["side"]).c_str());
    joined_game.state = std::shared_ptr<GameState>(new GameState(dynamic_cast< JSONObject & >( *value["game_state"] )));
    return joined_game;
  }
  return joined_game;
}

//blocking
std::shared_ptr<GameState> BattleshipsRCP::create_game()
{
  JSONObject message = ClientMessageMaker::make_create_game_message();
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::CREATE_GAME_RESPONSE)
  {
    JSONObject value = dynamic_cast< JSONObject & >( *object["message_value"] );
    std::shared_ptr<GameState> game_state(new GameState(dynamic_cast< JSONObject & >( value )));
    return std::shared_ptr<GameState>(game_state);
  }
  return std::shared_ptr<GameState>(nullptr);
}

//non-blocking
void BattleshipsRCP::send_chat(std::string chat_message)
{
  JSONObject message = ClientMessageMaker::make_chat_message(chat_message);
  message_sender->send_message(message.to_string());
}

void BattleshipsRCP::quit_game()
{
  JSONObject message = ClientMessageMaker::make_quit_message();
  message_sender->send_message(message.to_string());
}

//int: save_game_id
//blocking
std::vector<std::pair<int, std::string>> BattleshipsRCP::get_saved_games()
{
  //TODO
  JSONObject message = ClientMessageMaker::make_get_save_game_message();
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  std::vector<std::pair<int, std::string>> saved_games;
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::GET_SAVED_GAMES_RESPONSE)
  {
    JSONArray value = dynamic_cast< JSONArray & >( *object["message_value"] );
    std::vector<JSONElement_ptr> values = value.get_elements();
    for ( auto it = values.begin(); it != values.end(); it++ )
    {
      JSONObject saved_game_json = dynamic_cast< JSONObject & >( ( **it ) );
      int save_game_id = atoi(JSONString::extract_string(saved_game_json["save_game_id"]).c_str());
      std::string name = JSONString::extract_string(saved_game_json["name"]).c_str();
      std::string timestamp = JSONString::extract_string(saved_game_json["timestamp"]).c_str();
      saved_games.push_back(std::pair<int, std::string>(save_game_id, name + " " + timestamp));
    }
  }
  return saved_games;
}

//int: turn of
//blocking
void BattleshipsRCP::load_game(int save_game_id)
{
  JSONObject message = ClientMessageMaker::make_load_game_message(save_game_id);
  message_sender->send_message(message.to_string());
}

//int: game_id
//blocking
std::vector<std::pair<int, std::string>> BattleshipsRCP::get_current_games()
{
  JSONObject message = ClientMessageMaker::make_get_all_games_message();
  message_sender->send_message(message.to_string());
  std::string response = message_sender->receive_message(true);
  JSONObject object = JSONObject::parse_object(response);
  std::vector<std::pair<int, std::string>> current_games;
  if ( JSONString::extract_string(object["message_type"]) == ServerMessages::GET_CURRENT_GAMES_RESPONSE )
  {
    JSONArray value = dynamic_cast< JSONArray & >( *object["message_value"] );
    std::vector<JSONElement_ptr> values = value.get_elements();
    for ( auto it = values.begin(); it != values.end(); it++ )
    {
      JSONObject current_game_json = dynamic_cast< JSONObject & >( ( **it ) );
      int game_id = atoi(JSONString::extract_string(current_game_json["game_id"]).c_str());
      std::string created_by = JSONString::extract_string(current_game_json["created_by"]).c_str();
      std::string wins = JSONString::extract_string(current_game_json["wins"]).c_str();
      std::string losses = JSONString::extract_string(current_game_json["losses"]).c_str();
      std::string num_players = JSONString::extract_string(current_game_json["num_players_in_game"]).c_str();
      std::string max_players = JSONString::extract_string(current_game_json["max_players_in_game"]).c_str();
      current_games.push_back(std::pair<int, std::string>(game_id, created_by + " Wins: " + wins + " Losses: " + losses + "    " + num_players + "/" + max_players));
    }
  }
  return current_games;

}

void BattleshipsRCP::submit_reef_configuration_decision(bool decision)
{
  JSONObject message = ClientMessageMaker::make_reef_response_message(decision);
  message_sender->send_message(message.to_string());
}

void BattleshipsRCP::submit_command(Command& command)
{
  JSONObject message = ClientMessageMaker::make_command_message(command);
  message_sender->send_message(message.to_string());
}

void BattleshipsRCP::submit_prep_phase_done()
{
  JSONObject message = ClientMessageMaker::make_prep_phase_done_message();
  message_sender->send_message(message.to_string());
}