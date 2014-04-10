#include "BattleShipsGui.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_net.h"
#include "SDL_ttf.h"
#include <time.h>
#include <memory>
#include <iostream>
#include <fstream>

#include "Fonts.h"
#include "TextureManager.h"

#include "Stage.h"
#include "Scene.h"
#include "LoginScene.h"
#include "MatchScene.h"

#include "ClientMessageMaker.h"
#include "ClientServerMessages.h"

const std::string ends_with = "[END]";

// Change from setting a value to false to throwing an exception.
BattleShipsGui::BattleShipsGui()
{
  srand(time(0));
  bool success = true;
  if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
  {
    fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    if ( !SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") )
    {
      fprintf(stderr, "Warning: VSync not enabled!");
    }
    if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") )
    {
      fprintf(stderr, "Warning: Linear texture filtering not enabled!");
    }
    screen = std::unique_ptr<Screen>(new Screen(30, 30));
    window = std::make_unique<Window>(SDL_CreateWindow("BattleShips Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      screen->get_width_in_pixels(), screen->get_height_in_pixels(), SDL_WINDOW_SHOWN));
    if ( window == NULL )
    {
      fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      renderer = std::make_unique<Renderer>(SDL_CreateRenderer(window->get_sdl_window(), -1, SDL_RENDERER_ACCELERATED));
      if ( renderer == NULL )
      {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(renderer->get_sdl_renderer(), 0, 0, 0, 0);
        int imgFlags = IMG_INIT_PNG;
        if ( !( IMG_Init(imgFlags) & imgFlags ) )
        {
          fprintf(stderr, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
          success = false;
        }
        if ( TTF_Init() == -1 )
        {
          fprintf(stderr, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
          success = false;
        }
        if ( SDLNet_Init() < 0 )
        {
          fprintf(stderr, "SDL_net could not initialize! SDL_net error: %s\n", SDLNet_GetError());
          success = false;
        }
      }
    }
  }
  Fonts::set_instance();
  TextureManager::set_instance(renderer, screen);
  Stage::set_instance(screen);
  connection = SDLNet_AllocSocketSet(1);
  if ( !connection )
  {
    success = false; // Another thing to be an exception instead.
  }
};

BattleShipsGui::~BattleShipsGui()
{
  Fonts::release_instance();
  if ( server_socket != NULL )
  {
    SDLNet_TCP_Close(server_socket);
  }

  //Quit SDL subsystems
  SDLNet_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
};

void BattleShipsGui::send_message(const std::string& message)
{
  int sent = 0;
  std::string new_message = message + ends_with;
  const char *data = ( new_message ).c_str();
  int data_length = new_message.length() + 1;
  fprintf(stderr, "Sending message: %s\n", data);
  if ( server_socket )
  {
    sent = SDLNet_TCP_Send(server_socket, data, data_length);
    if ( sent < data_length )
    {
      fprintf(stderr, "Sending message failed; bytes sent: %d, bytes expected: %d.\n", sent, data_length);
    }
  }
  else
  {
    fprintf(stderr, "Socket not open, send failed.\n");
  }
};

void BattleShipsGui::run()
{
  std::unique_ptr<Stage> const &stage = Stage::get_instance();
  // Get address to game server and port.
  // Default to localhost at default_port (9999), if no other source provided.
  std::string game_server_address = "localhost";
  uint16_t game_server_port = 9999;
  std::ifstream cfg;
  cfg.open("battleships.cfg");
  if ( !cfg.fail() )
  {
    char buffer[101];
    cfg.getline(buffer, 101);
    game_server_address = buffer;
    cfg.getline(buffer, 101);
    game_server_port = atoi(buffer);
    std::cout << "Server Address: " << game_server_address << std::endl;
    std::cout << "Server Port: " << game_server_port << std::endl;
  }
  else
  {
    std::cerr << "Error opening config file, using default values." << std::endl;
  }
  cfg.close();

  connect_server(game_server_address.c_str(), game_server_port);
  SDLNet_TCP_AddSocket(connection, server_socket);

  char connection_message[50000];
  memset(( void * ) connection_message, 0, 50000);
  if ( server_socket == NULL || SDLNet_TCP_Recv(server_socket, connection_message, 50000) <= 0 )
  {
    fprintf(stderr, "Error receiving data from the server!\n");
  }
  fprintf(stderr, "%s\n", connection_message);
  if ( !server_socket )
  {
    std::cerr << "No server connection could be made, local play enabled." << std::endl;
    stage->push_scene(std::unique_ptr<Scene>(new MatchScene(( MessageSender * ) this)));
  }
  else
  {
    std::cout << "Connected to server." << std::endl;
    stage->push_scene(std::unique_ptr<Scene>(new LoginScene(( MessageSender * ) this)));
  }

  SDL_Event e;
  bool quit = false;
  int frameStart, frameTime;
  while ( !quit )
  {
    stage->update();
    frameStart = SDL_GetTicks();
    while ( SDL_PollEvent(&e) )
    {
      if ( !( stage->has_scene() ) )
      {
        quit = true;
      }
      stage->handle_event(e);
    }
    if ( server_socket != NULL )
    {
      std::string message = receive_message(false);
      if ( !message.empty() )
      {
        handle_server_message(message);
      }
    }
    SDL_RenderClear(renderer->get_sdl_renderer());
    stage->render();
    SDL_RenderPresent(renderer->get_sdl_renderer());
    frameTime = SDL_GetTicks() - frameStart;
    if ( frameTime < DELAY_TIME )
    {
      SDL_Delay((int)( DELAY_TIME - frameTime ));
    }
  }
};

bool BattleShipsGui::connect_server(const char *server_host, const uint16_t port)
{
  bool ret = false;
  IPaddress ip;

  ret = ( SDLNet_ResolveHost(&ip, server_host, port) ) == 0;

  if ( ret )
  {
    server_socket = SDLNet_TCP_Open(&ip);
  }
  else
  {
    fprintf(stderr, "Error resolving host: %s\n", SDLNet_GetError());
  }

  ret = ( NULL != server_socket );
  if ( !ret )
  {
    fprintf(stderr, "Error opening socket: %s\n", SDLNet_GetError());
  }

  return ret;
};


std::string BattleShipsGui::receive_message(bool blocking)
{
  // Receive events from the network and deliver them.
  char buffer[1001];
  std::string received_message = "";
  do 
  {
    if ( SDLNet_CheckSockets(connection, 5) )
    {
      int recv = 0;
      int start_at = received_message.size() - ends_with.size();
      do
      {
        memset(( void * ) buffer, 0, 1001);
        recv = SDLNet_TCP_Recv(server_socket, buffer, 1000);
        received_message = received_message + buffer;
        start_at = received_message.size() - ends_with.size();
        std::cout << buffer << std::endl;
      } while ( ("}" + ends_with) != received_message.substr(std::max(0, start_at - 1), ends_with.size() + 1) );
      break;
    }
  } while ( blocking );
  return received_message.substr(0, received_message.size() - ends_with.size());
}

void BattleShipsGui::handle_server_message(std::string message)
{
  if ( message.empty() ) { return; }
  JSONObject response_object = JSONObject::parse_object(message);
  const std::string type = JSONString::extract_string(response_object["message_type"]);

  if ( type == ClientMessages::CHAT_MESSAGE )
  {
    const std::string value = JSONString::extract_string(response_object["message_value"]);
    const std::string from = JSONString::extract_string(response_object["from"]);
    Stage::get_instance()->deliver_chat_message(from, value);
  }
  else if ( type == ClientMessages::EXECUTE_COMMAND )
  {
    JSONObject value = dynamic_cast<JSONObject &>(* response_object["message_value"]);
    Command c = Command::unserialize(value);
    Stage::get_instance()->deliver_command(c);
  }
  else if ( type == ServerMessages::NOTIFY_PLAYER_JOINED_GAME )
  {
    //can start prep phase
    std::cout << "Player 2 joined";
    JSONObject value = dynamic_cast<JSONObject &>( *response_object["message_value"] );
    std::string username = JSONString::extract_string(value["username"]);
    std::string wins = JSONString::extract_string(value["wins"]);
    std::string losses = JSONString::extract_string(value["losses"]);
    Stage::get_instance()->deliver_notify_other_player_joined_game(username + " (W:" + wins + ",L:" + losses + ")");
  }
  else if ( type == ServerMessages::REEF_CONFIG_ANSWER )
  {
    JSONObject value = dynamic_cast<JSONObject &>( *response_object["message_value"] );
    bool reef_was_accepted = JSONString::extract_string(value["accept_reef"]) == ServerMessages::SUCCESS ? true : false;
    if ( reef_was_accepted )
    {
      Stage::get_instance()->deliver_reef_phase_over();
    }
    else
    {
    std::vector<JSONElement_ptr> reef_array = dynamic_cast< JSONArray & >( *value["reef_config"] ).get_elements();
    std::vector<Reef> reef_config;
    for ( auto it = reef_array.begin(); it != reef_array.end(); it++ )
    {
      reef_config.push_back(Reef::unserialize(dynamic_cast< JSONObject & >( **it )));
    }
      Stage::get_instance()->deliver_new_reefs(reef_config);
    }
    //yes or no + reef config
  }
  else if ( type == ServerMessages::PREP_PHASE_DONE )
  {
    //can start game
    Stage::get_instance()->deliver_ship_setup_phase_over();
  }
  else if ( type == ServerMessages::NOTIFY_PLAYER_QUIT_GAME )
  {
    Stage::get_instance()->deliver_other_player_quit_game();
  }
  else if ( type == ServerMessages::NOTIFY_GAME_LOADED )
  {
    JSONObject value = dynamic_cast< JSONObject & >( *response_object["message_value"] );
    Player::Side turn_of = ( Player::Side ) atoi(JSONString::extract_string(value["turn_of"]).c_str());
    Player::Side side = static_cast<Player::Side>( atoi(JSONString::extract_string(value["side"]).c_str()) );
    std::shared_ptr<GameState> game_state(new GameState(dynamic_cast< JSONObject & >( *value["game_state"] )));
    Stage::get_instance()->deliver_match_loaded(turn_of, side, game_state, Enums::GAMEPLAY);
  }
  else if ( type == ServerMessages::NOTIFY_END_GAME )
  {
    Player::Side winner = static_cast<Player::Side>( atoi(JSONString::extract_string(response_object["message_value"]).c_str()) );
    Stage::get_instance()->deliver_has_match_ended(winner);
  }
}