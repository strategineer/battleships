#include "ClientMessageMaker.h"
#include "ClientServerMessages.h"

JSONObject ClientMessageMaker::make_command_message(Command& command)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::EXECUTE_COMMAND)));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(command.serialize())));
  return message;
}

JSONObject ClientMessageMaker::make_chat_message(std::string chat_message)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::CHAT_MESSAGE)));
  message.set_value("message_value", JSONElement_ptr(new JSONString(chat_message)));
  return message;
}

JSONObject ClientMessageMaker::make_quit_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::QUIT_GAME)));
  return message;
}

JSONObject ClientMessageMaker::make_reef_response_message(bool accept_reef)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::RESPONSE_REEF_CONFIG)));
  message.set_value("message_value", JSONElement_ptr(new JSONString(std::to_string(accept_reef))));
  return message;
}


JSONObject ClientMessageMaker::make_save_game_message(std::string save_game_name)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::SAVE_GAME)));
  message.set_value("message_value", JSONElement_ptr(new JSONString(save_game_name)));
  return message;
}

JSONObject ClientMessageMaker::make_create_game_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::CREATE_GAME)));
  return message;
}

JSONObject ClientMessageMaker::make_get_save_game_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::GET_SAVED_GAMES)));
  return message;
}

JSONObject ClientMessageMaker::make_get_all_games_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::GET_ALL_GAMES)));
  return message;
}

JSONObject ClientMessageMaker::make_register_user_message(std::string user, std::string password)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::REGISTER_USER)));
  JSONObject value;
  value.set_value("user", JSONElement_ptr(new JSONString(user)));
  value.set_value("password", JSONElement_ptr(new JSONString(password)));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(value)));
  return message;
}

JSONObject ClientMessageMaker::make_login_user_message(std::string user, std::string password)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::LOGIN_USER)));
  JSONObject value;
  value.set_value("user", JSONElement_ptr(new JSONString(user)));
  value.set_value("password", JSONElement_ptr(new JSONString(password)));
  message.set_value("message_value", JSONElement_ptr(new JSONObject(value)));
  return message;
}

JSONObject ClientMessageMaker::make_load_game_message(int save_game_id)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::LOAD_GAME)));
  message.set_value("message_value", JSONElement_ptr(new JSONString(std::to_string(save_game_id))));
  return message;
}
JSONObject ClientMessageMaker::make_join_game_message(int game_id)
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::JOIN_GAME)));
  message.set_value("message_value", JSONElement_ptr(new JSONString(std::to_string(game_id))));
  return message;
}

JSONObject ClientMessageMaker::make_prep_phase_done_message()
{
  JSONObject message;
  message.set_value("message_type", JSONElement_ptr(new JSONString(ClientMessages::PREP_PHASE_DONE)));
  return message;
}