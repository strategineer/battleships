#pragma once
#include <queue>
#include <mutex>

class MessageQueue
{
private:
  std::queue<std::string> message_queue;
  std::mutex mqueue_mutex;
public:
  MessageQueue();
  ~MessageQueue();
  bool is_empty();
  std::string dequeue();
  void enqueue(std::string some_message);
};