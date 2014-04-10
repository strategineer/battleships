#include "ServerUser.h"

ServerUser::ServerUser(TCPsocket socket) : user_handler(std::shared_ptr<MessageHandler>(new MessageHandler(socket)))
{
  user_handler->start_listening();
  username = "";
}

ServerUser::~ServerUser()
{

}

bool ServerUser::has_message()
{
  return user_handler->has_message();
}

std::string ServerUser::retrieve_message()
{
  return user_handler->retrieve_message();
}

bool ServerUser::send_message(std::string message)
{
  send_mutex.lock();
  bool b = user_handler->send_message(message);
  send_mutex.unlock();
  return b;
}

bool ServerUser::is_connected()
{
  return user_handler->is_connected();
}

void ServerUser::set_username(std::string user)
{
  username = user;
}

std::string ServerUser::get_username()
{
  return username;
}