#pragma once
#include "SDL.h"
#include <string>
#include <vector>
#include <memory>
#include "Renderable.h"
#include "Notification.h"
#include "Point.h"
#include "Ping.h"
/* Manages notifications displayed to the player. */
class NotificationManager
  : public Renderable
{
public:
  NotificationManager();
  ~NotificationManager();
  /* Add a new notification to show to the player. */
  void display_notification(std::string const msg, int time_in_ms);
  void display_notification(std::string const msg, int time_in_ms, const SDL_Color color);
  /* Add a new ping to show to the player. */
  void display_ping(const Point & p, int time_in_ms);
  /* Deletes notifications which have expired. */
  void update();
private:
  void do_render();
  /* Number of notifications displayed since last time there were zero notifications. */
  int stack_size;
  Texture ping;
  std::vector<std::unique_ptr<Notification>> notifications;
  std::vector<std::unique_ptr<Ping>> pings;
};

