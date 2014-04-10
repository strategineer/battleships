#include "SceneAction.h"


SceneAction::SceneAction(SceneEvent e, std::unique_ptr<Scene> &scn)
{
  event = e;
  scene = std::move(scn);
}


SceneAction::~SceneAction()
{}
