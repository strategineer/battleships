#pragma once
#include "MessageHandler.h"
#include <memory>
#include <mutex>

class ServerUser
{
private:
  std::shared_ptr<MessageHandler> user_handler;
  std::string username;
  std::mutex send_mutex; //no need for receive mutex
public:
  ServerUser(TCPsocket socket);
  ~ServerUser();
  bool has_message();
  std::string retrieve_message();
  bool send_message(std::string message);
  bool is_connected();
  void set_username(std::string user);
  std::string get_username();

};