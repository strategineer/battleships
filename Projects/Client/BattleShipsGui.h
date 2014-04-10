#pragma once
#include <memory>
#include "SDL_net.h"

#include "MessageSender.h"
#include "Window.h"
#include "Renderer.h"
#include "Screen.h"
/*
Overarching class encapsulating all of the GUI
*/
class BattleShipsGui : public MessageSender
{
public:
  BattleShipsGui();
  ~BattleShipsGui();

  virtual void send_message(const std::string& message) override;
  virtual std::string receive_message(bool blocking) override;

  /* Run/Start the GUI */
  void run();
private:
  static const int DELAY_TIME = 1000.0f / 60;
  static const uint16_t default_port = 9999;

  bool connect_server(const char *server_address, const uint16_t port);

  void handle_server_message(std::string message);

  std::unique_ptr<Window> window;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<Screen> screen;

  SDLNet_SocketSet connection;
  TCPsocket server_socket;

};

