#pragma once
#include "SDL.h"
#include "EventHandler.h"
#include "Renderable.h"
/* 
  Interface, represents a particular visual interface that provides 
  information to the player and reacts his input. 
  */
class Scene
  : public EventHandler, public Renderable
{
public:
  virtual ~Scene() = default;
  /* Update the scene. Should be called before this.render is called and only once per. */
  void update() { do_update(); }
  /* Called when switching to this scene from another scene. */
  void on_enter() { do_on_enter(); }
  /* Called when switching to another scene from this scene. */
  void on_exit() { do_on_exit(); }
private:
  virtual void do_update() = 0;
  virtual void do_on_enter() = 0;
  virtual void do_on_exit() = 0;
};

