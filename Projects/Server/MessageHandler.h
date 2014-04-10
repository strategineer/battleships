#pragma once
#include "MessageQueue.h"
#include "SDL_net.h"
#include "SDL_thread.h"

class MessageHandler
{
private:
  MessageQueue message_queue;
  int server_port;
  char server_address[50];
  TCPsocket message_socket;
  static char *default_server_address;
  static int default_server_port;
  bool connection_open;
  bool listening;
  static int receive_messages(void *some_queue);
  static int send_messages(void* nothing);
  std::queue<std::string> send_message_buffer;
  std::mutex send_mutex_buffer;
  SDL_Thread *sender_thread;
public:
  MessageHandler(const char *pserver_address, const int pserver_port);
  MessageHandler(TCPsocket pmessage_socket);
  ~MessageHandler();
  bool start_listening();
  bool connect();
  bool has_message();
  std::string retrieve_message();
  bool send_message(std::string message);
  bool is_connected();

};