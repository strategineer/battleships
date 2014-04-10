#pragma once
#include "SDL_net.h"
#include <map>
#include <memory>

#include "CurrentGame.h"
#include "ServerUser.h"
#include "JSONObject.h"
#include <set>

class Server
{
private:
  int server_port;
  std::map<unsigned int, std::shared_ptr<CurrentGame>> active_games;
  std::map<std::string, unsigned int> user_to_game_id;
  std::set<std::string> users_online;
  // Invoked to launch the handler; see create_handler_thread() for usage.
  static int s_thread_handler(void *data);
  int handler(void *data);
  void create_handler_thread(TCPsocket socket);
  void handle_client_message(std::shared_ptr<ServerUser> user_ptr, JSONObject client_message);
  void user_quit_game(std::string username);
  void close_game(std::shared_ptr<CurrentGame> current_game);

public:
  Server(int port);
  int listen();
  ~Server();
};