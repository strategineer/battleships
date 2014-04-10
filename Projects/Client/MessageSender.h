#pragma once
#include <string>

class MessageSender
{
public:
  virtual void send_message(const std::string& message) = 0;
  virtual std::string receive_message(bool blocking) = 0;

};