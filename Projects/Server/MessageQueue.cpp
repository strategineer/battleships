#include "MessageQueue.h"

MessageQueue::MessageQueue()
{}

MessageQueue::~MessageQueue()
{}

std::string MessageQueue::dequeue()
{
  std::string some_message;
  mqueue_mutex.lock();
  if ( !message_queue.empty() )
  {
    some_message = message_queue.front();
    message_queue.pop();
  }
  mqueue_mutex.unlock();
  return some_message;
}

void MessageQueue::enqueue(std::string some_message)
{
  mqueue_mutex.lock();
  message_queue.push(some_message);
  mqueue_mutex.unlock();
}

bool MessageQueue::is_empty()
{
  bool is_empty = true;

  mqueue_mutex.lock();
  is_empty = message_queue.empty();
  mqueue_mutex.unlock();

  return is_empty;
}

