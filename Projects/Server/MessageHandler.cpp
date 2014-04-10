#include "MessageHandler.h"
#include "SDL_thread.h"
#include <iostream>
#include <chrono>
#include <thread>
const int buffer_size = 50000;
const std::string ends_with = "[END]";
MessageHandler::MessageHandler(const char *pserver_address, const int pserver_port) :
server_port(pserver_port), connection_open(false), listening(false)
{
  strcpy_s(server_address, 50, pserver_address);
}

MessageHandler::MessageHandler(TCPsocket pmessage_socket) :
message_socket(pmessage_socket), connection_open(true), listening(false)
{

}

MessageHandler::~MessageHandler()
{
  int status = 0;
  SDL_WaitThread(sender_thread, &status);
  SDLNet_TCP_Close(message_socket);
}

bool MessageHandler::connect()
{
  IPaddress ip;
  message_socket = NULL;
  if ( SDLNet_ResolveHost(&ip, server_address, server_port) != -1 )
  {
    message_socket = SDLNet_TCP_Open(&ip);
    if ( message_socket )
    {
      connection_open = true;
      return true;
    }
  }
  return false;
}

bool MessageHandler::start_listening()
{
  if ( listening )
  {
    return false;
  }
  listening = true;
  SDL_Thread *thread = SDL_CreateThread(receive_messages, "receive messages", ( void * ) this);
  return true;
}

bool MessageHandler::has_message()
{
  return !message_queue.is_empty();
}

std::string MessageHandler::retrieve_message()
{
  return message_queue.dequeue();
}

int MessageHandler::send_messages(void* handler)
{
  MessageHandler* m_handler = ( MessageHandler *) handler;
  while ( true )
  {
    m_handler->send_mutex_buffer.lock();
    if ( m_handler->send_message_buffer.empty() )
    {
      m_handler->send_mutex_buffer.unlock();
      break;
    }
    if ( m_handler->connection_open && m_handler->message_socket )
    {
      std::string message = m_handler->send_message_buffer.front() + ends_with;
      int len, result;
      len = message.length() + 1;
      result = SDLNet_TCP_Send(m_handler->message_socket, message.c_str(), len);
      m_handler->send_mutex_buffer.unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(std::max(200, (len / 3000) * 1000)));
      m_handler->send_message_buffer.pop();
      if ( result < len )
      {
        break;
      }
    }
    else
    {
      m_handler->send_mutex_buffer.unlock();
      break;
    }
  }
  return 0;
}

bool MessageHandler::send_message(std::string message)
{
  send_mutex_buffer.lock();
  if ( send_message_buffer.empty() )
  {
    sender_thread = SDL_CreateThread(send_messages, "send messages", ( void * ) this);
  }
  send_message_buffer.push(message);
  send_mutex_buffer.unlock();
  return true;
}

int MessageHandler::receive_messages(void *handler)
{
  MessageHandler* message_handler = (MessageHandler *)handler;
  while ( message_handler->connection_open )
  {
    std::string message;
    int start_at = message.size() - ends_with.size();
    while ( ("}" + ends_with) != message.substr(std::max(0, start_at - 1), ends_with.size() + 1) )
    {
      char buffer[buffer_size];
      if ( SDLNet_TCP_Recv(message_handler->message_socket, buffer, buffer_size) <= 0 )
      {
        //Disconnected
        message_handler->connection_open = false;
        break;
      }
      message = message + std::string(buffer);
      start_at = message.size() - ends_with.size();
    }
    message_handler->message_queue.enqueue(message.substr(0, message.size() - ends_with.size()));
  }
  message_handler->listening = false;
  return 0;
}

bool MessageHandler::is_connected()
{
  return connection_open;
}