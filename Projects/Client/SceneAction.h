#pragma once
#include <memory>
#include "Scene.h"

class SceneAction
{
public:
  enum class SceneEvent
  {
    Push,
    Pop,
    Switch,
    Quit
  };
  SceneAction(SceneEvent e, std::unique_ptr<Scene> &scn);
  ~SceneAction();
  std::unique_ptr<Scene> scene;
  SceneEvent event;
};

