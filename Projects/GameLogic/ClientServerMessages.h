#pragma once
#include <string>

namespace ServerMessages
{
  const std::string REGISTER_RESPONSE = "REGISTER_RESPONSE";
  const std::string LOGIN_RESPONSE = "LOGIN_RESPONSE";
  const std::string CREATE_GAME_RESPONSE = "CREATE_GAME_RESPONSE";
  const std::string SAVE_GAME_RESPONSE = "SAVE_GAME_RESPONSE";
  const std::string CREATE_GAME_ANSWER = "CREATE_GAME_RESPONSE";
  const std::string JOIN_GAME_RESPONSE = "JOIN_GAME_RESPONSE";
  const std::string NOTIFY_PLAYER_JOINED_GAME = "NOTIFY_PLAYER_JOINED_GAME";
  const std::string GET_SAVED_GAMES_RESPONSE = "GET_SAVED_GAMES_RESPONSE";
  const std::string LOAD_GAME_RESPONSE = "LOAD_GAME_RESPONSE";
  const std::string GET_CURRENT_GAMES_RESPONSE = "GET_CURRENT_GAMES_RESPONSE";
  const std::string REEF_CONFIG_ANSWER = "REEF_CONFIG_ANSWER";
  const std::string PREP_PHASE_DONE = "PREP_PHASE_DONE";
  const std::string NOTIFY_END_GAME = "NOTIFY_END_GAME";
  const std::string SUCCESS = "SUCCESS";
  const std::string FAILURE = "FAILURE";
  const std::string NONE = "NONE";
  const std::string CANNOT_JOIN_GAME = "CANNOT_JOIN_GAME";
  const std::string NOTIFY_PLAYER_QUIT_GAME = "NOTIFY_PLAYER_QUIT_GAME";
  const std::string NOTIFY_GAME_LOADED = "NOTIFY_GAME_LOADED";
}

namespace ClientMessages
{
  const std::string EXECUTE_COMMAND = "EXECUTE_COMMAND";
  const std::string CHAT_MESSAGE = "CHAT_MESSAGE";
  const std::string RESPONSE_REEF_CONFIG = "RESPONSE_REEF_CONFIG";
  const std::string SAVE_GAME = "SAVE_GAME";
  const std::string CREATE_GAME = "CREATE_GAME";
  const std::string GET_SAVED_GAMES = "GET_SAVED_GAMES";
  const std::string GET_ALL_GAMES = "GET_ALL_GAMES";
  const std::string REGISTER_USER = "REGISTER_USER";
  const std::string LOGIN_USER = "LOGIN_USER";
  const std::string LOAD_GAME = "LOAD_GAME";
  const std::string JOIN_GAME = "JOIN_GAME";
  const std::string PREP_PHASE_DONE = "PREP_PHASE_DONE";
  const std::string QUIT_GAME = "QUIT_GAME";
  const std::string GAME_WON = "GAME_WON";
}
