#include "Server.h"
#include "SDL_thread.h"
#include <iostream>
#include <memory>
#include <map>
#include "MessageHandler.h"
#include "CurrentGame.h"
#include "JSON.h"
#include "BattleshipsDataSystem.h"
#include "ClientRequestValues.h"
#include "ServerMessageMaker.h"
int default_port = 9999;
bool stop = false;

void start_server()
{
  Server server(default_port);
  server.listen();
};


int main(int argc, char* args[])
{
  BattleshipsDataSystem::init_database();
  start_server();
  return 0;
};
