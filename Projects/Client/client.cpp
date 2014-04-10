#include "BattleShipsGui.h"
#include <memory>
int main(int argc, char* args[])
{
  std::unique_ptr<BattleShipsGui> gui(new BattleShipsGui);
  gui->run();
  return 0;
}