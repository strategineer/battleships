#include "NotificationManager.h"
#include <algorithm>


NotificationManager::NotificationManager()
{
  stack_size = 0;
  ping.load_from_file("highlight/notification.png");
}


NotificationManager::~NotificationManager()
{}

void NotificationManager::display_notification(std::string const msg, int time_in_ms)
{
  notifications.push_back(std::unique_ptr<Notification>(new Notification(msg, time_in_ms)));
  stack_size++;
  notifications.at(notifications.size() - 1)->get_texture()->set_position_by_screen_factor(0.5, 0.95);
  notifications.at(notifications.size() - 1)->get_texture()->shift_position(0, -40 * stack_size);
}

void NotificationManager::display_notification(std::string const msg, int time_in_ms, const SDL_Color color)
{
  notifications.push_back(std::unique_ptr<Notification>(new Notification(msg, time_in_ms, color)));
  stack_size++;
  notifications.at(notifications.size() - 1)->get_texture()->set_position_by_screen_factor(0.5, 0.95);
  notifications.at(notifications.size() - 1)->get_texture()->shift_position(0, -40 * stack_size);
}


void NotificationManager::display_ping(const Point & p, int time_in_ms)
{
  pings.push_back(std::unique_ptr<Ping>(new Ping(p, time_in_ms)));
}

void NotificationManager::update()
{
  if ( pings.size() > 0 )
  {
    bool needs_cleanup = false;
    for ( auto & p : pings )
    {
      if ( p->has_expired() )
      {
        needs_cleanup = true;
        break;
      }
    }
    if ( needs_cleanup )
    {
      pings.erase(
        std::remove_if(
        pings.begin(),
        pings.end(),
        [&](std::unique_ptr<Ping>& i){return i->has_expired(); }),
        pings.end());
    }
  }

  if ( notifications.size() > 0 )
  {
    bool needs_cleanup = false;
    for ( auto & notif : notifications )
    {
      if ( notif->has_expired() )
      {
        needs_cleanup = true;
        break;
      }
    }
    if ( needs_cleanup )
    {
      notifications.erase(
        std::remove_if(
        notifications.begin(),
        notifications.end(),
        [&](std::unique_ptr<Notification>& i){return i->has_expired(); }),
        notifications.end());
    }
  }
  else
  {
    stack_size = 0;
  }

  if ( stack_size > 14 )
  {
    stack_size = 0;
  }
}

void NotificationManager::do_render()
{
  for ( auto & notif : notifications )
  {
    notif->render();
  }
  for ( auto & p : pings )
  {
    const Point & loc = p->get_location();
    ping.set_position_on_grid(loc.get_x(), loc.get_y());
    ping.render();
  }
}