#include "SDL_net.h"
#include "SDL_thread.h"
#include <iostream>
#include "Server.h"
#include "BattleshipsDataSystem.h"
#include "ClientRequestValues.h"
#include "CurrentGame.h"
#include "ServerMessageMaker.h"

Server::Server(int port) : server_port(port)
{}

int Server::listen()
{
  IPaddress address;
  TCPsocket server_socket;
  //initialize the server
  SDLNet_Init();
  if ( SDLNet_ResolveHost(&address, NULL, server_port) != 0 )
  {
    return -1;
  }
  server_socket = SDLNet_TCP_Open(&address);
  if ( !server_socket )
  {
    return -1;
  }

  //listen for incomming connections
  TCPsocket new_socket;
  while ( true )
  {
    new_socket = SDLNet_TCP_Accept(server_socket);
    if ( new_socket )
    {
      //when a new connection is established create a new thread
      fprintf(stderr, "Opened new connection.\n");
      std::string connection_text = "Welcome to Battleships!";
      SDLNet_TCP_Send(new_socket, connection_text.c_str(), connection_text.length());
      create_handler_thread(new_socket);
      new_socket = NULL;
    }
  }
  return 0;
}

//Create a thread to handle the client
void Server::create_handler_thread(TCPsocket socket)
{
  SDL_Thread *thread;
  auto args = new std::pair<Server*, TCPsocket>(this, socket);
  //Give the tuple as parameter of the fn
  thread = SDL_CreateThread(&Server::s_thread_handler, "Client", (void*) args);
}

int Server::s_thread_handler(void *data)
{
  auto args = (std::pair<Server*, TCPsocket> *) ( data );
  Server * server = args->first;
  TCPsocket socket = args->second;
  delete data;
  return static_cast<Server *> (server)->handler(socket);
};

int Server::handler(void *data)
{
  std::shared_ptr<ServerUser> user_ptr(new ServerUser((TCPsocket) data));
  //CurrentGame::UserType type = game->add_user(user_ptr);
  while (user_ptr->is_connected())
  {
    if (user_ptr->has_message())
    {
      std::string received = user_ptr->retrieve_message();
      if ( received != "" )
      {
        handle_client_message(user_ptr, JSONObject::parse_object(received));
      }
    }
  }
  user_quit_game(user_ptr->get_username());
  users_online.erase(user_ptr->get_username());
  std::cout << "Client disconnected." << std::endl;
  return 0;
};

void Server::user_quit_game(std::string username)
{
  if ( user_to_game_id.count(username))
  {
    auto current_game = active_games.at(user_to_game_id.at(username));
    bool is_player = current_game->is_player(username);
    if ( is_player )
    {
      current_game->lock_game();
      std::string notify = ServerMessageMaker::make_notify_quit_game_message().to_string();
      current_game->send_all(notify);
      close_game(current_game);
      current_game->unlock_game();
    }
    else
    {
      current_game->remove_spectator(username);
      user_to_game_id.erase(username);
    }
  }
}

void Server::close_game(std::shared_ptr<CurrentGame> current_game)
{
  std::string player_one = current_game->get_player_one();
  std::string player_two = current_game->get_player_two();
  std::vector<std::string> spectators = current_game->get_spectators();
  active_games.erase(current_game->get_game_id());
  user_to_game_id.erase(player_one);
  user_to_game_id.erase(player_two);
  for ( auto it = spectators.begin(); it != spectators.end(); it++ )
  {
    user_to_game_id.erase(*it);
  }
}

void Server::handle_client_message(std::shared_ptr<ServerUser> user_ptr, JSONObject client_message)
{
  std::string type = JSONString::extract_string(client_message["message_type"]);
  if (type == ClientRequestValues::CHAT_MESSAGE)
  {
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    JSONObject new_message = client_message;
    new_message.set_value("from", JSONElement_ptr(new JSONString(username)));
    current_game->send_all_except_me(user_ptr, new_message.to_string());
    //TODO: Forward message    
  }
  else if (type == ClientRequestValues::CREATE_GAME)
  {
    auto current_game = std::shared_ptr<CurrentGame>(new CurrentGame());
    current_game->lock_game();
    current_game->add_user(user_ptr);
    std::string response = ServerMessageMaker::make_create_game_response_message(current_game->get_game_state()).to_string();
    int size = response.size();
    user_ptr->send_message(response);
    int game_id = current_game->get_game_id();
    user_to_game_id.emplace(user_ptr->get_username(), game_id);
    active_games.emplace(game_id, current_game);
    current_game->unlock_game();
  }
  else if (type == ClientRequestValues::EXECUTE_COMMAND)
  {
    //TODO: Deliver command
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    current_game->lock_game();
    current_game->apply_to_state(user_ptr, dynamic_cast< JSONObject & >(*client_message["message_value"]));
    current_game->toggle_turn();
    current_game->send_all_except_me(user_ptr, client_message.to_string());
    std::pair<bool, Player::Side> game_won = current_game->game_is_won();
    if ( game_won.first )
    {
      std::string notify = ServerMessageMaker::make_notify_end_game_message(game_won.second).to_string();
      current_game->send_all(notify);
      BattleshipsDataSystem::record_completed_game(current_game->get_game_id(), game_won.second);
      //close_game(current_game);
    }
    current_game->unlock_game();
    //CHECK IF GAME HAS ENDED. IF YES SEND NOTIFICATION TO USERS
  }
  else if (type == ClientRequestValues::GET_ALL_GAMES)
  {
    std::vector<GameInfo> all_games;
    for (auto it = active_games.begin(); it != active_games.end(); it++)
    {
      all_games.push_back((*it).second->get_game_info());
    }
    std::string response = ServerMessageMaker::make_get_current_games_response_message(all_games).to_string();
    user_ptr->send_message(response);
    //TODO: Return list of all games waiting for player
  }
  else if (type == ClientRequestValues::GET_SAVED_GAMES)
  {
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    std::vector<SavedGame> all_saved_games = BattleshipsDataSystem::get_saved_games_of(current_game->get_player_one(), current_game->get_player_two());
    std::string response = ServerMessageMaker::make_get_save_games_response_message(all_saved_games).to_string();
    user_ptr->send_message(response);
    //TODO: Return list of all games waiting for player
  }
  else if (type == ClientRequestValues::JOIN_GAME)
  {
    //TODO: Add player to game. call create_new_game(..) on BattleshipsDataSystem which returns a game_id. Move game to the active games list.
    int game_id = atoi(JSONString::extract_string(client_message["message_value"]).c_str());
    try
    {
      auto current_game = active_games.at(game_id);
      user_to_game_id.emplace(user_ptr->get_username(), game_id);
      current_game->lock_game();
      Player::Side side = current_game->add_user(user_ptr);
      std::string response = ServerMessageMaker::make_join_game_response_message(false, current_game->get_turn_of(), side, current_game->get_game_state()).to_string();
      user_ptr->send_message(response);
      if ( side != Player::Side::SPECTATOR )
      {
        std::pair<int, int> wins_losses = BattleshipsDataSystem::get_wins_and_losses_of(user_ptr->get_username());
        std::string notification = ServerMessageMaker::make_notify_player_joined_game_message(user_ptr->get_username(), wins_losses.first, wins_losses.second).to_string();
        current_game->send_all_except_me(user_ptr, notification);
      }
      current_game->unlock_game();
    }
    catch ( std::out_of_range )
    {
      std::string response = ServerMessageMaker::make_join_game_response_message(true, Player::Side::PLAYER_ONE, Player::Side::PLAYER_ONE, nullptr).to_string();
      user_ptr->send_message(response);
    }
  }
  else if (type == ClientRequestValues::LOAD_GAME)
  {
    int saved_game_id = atoi(JSONString::extract_string(client_message["message_value"]).c_str());
    LoadedGame lg = BattleshipsDataSystem::load_game(saved_game_id);
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    current_game->lock_game();
    active_games.erase(current_game->get_game_id());
    std::string player_one = current_game->get_player_one();
    std::string player_two = current_game->get_player_two();
    std::vector<std::string> spectators = current_game->get_spectators();
    current_game->load_game(lg);
    // Add loaded game state to server
    active_games.emplace(lg.new_game_id, current_game);

    user_to_game_id.erase(player_one);
    user_to_game_id.erase(player_two);
    user_to_game_id.emplace(player_one, lg.new_game_id);
    user_to_game_id.emplace(player_two, lg.new_game_id);

    for ( auto it = spectators.begin(); it != spectators.end(); it++ )
    {
      user_to_game_id.erase(*it);
      user_to_game_id.emplace(*it, lg.new_game_id);
    }
    current_game->unlock_game();
    //must remove old game from active_games
    // Attach user to game?
    //TODO: Load game. call load_game(...) on battleshipsDataSystem which returns a LoadedGame. Send game_state to both players (and spectators) with the
  }
  else if (type == ClientRequestValues::LOGIN_USER)
  {
    JSONObject message_value = dynamic_cast< JSONObject & >(*client_message["message_value"]);
    std::string user = JSONString::extract_string(message_value["user"]);
    std::string password = JSONString::extract_string(message_value["password"]);
    bool is_auth = false;
    if ( std::find(users_online.begin(), users_online.end(), user) == users_online.end() )
    {
      is_auth = BattleshipsDataSystem::authenticate_user(user, password);
    }
    std::string response = ServerMessageMaker::make_login_response_message(is_auth).to_string();
    std::cout << response;
    if (is_auth)
    {
      user_ptr->set_username(user);
      users_online.emplace(user);
    }
    user_ptr->send_message(response);

  }
  else if (type == ClientRequestValues::PREP_PHASE_DONE)
  {
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    current_game->lock_game();
    bool ready = current_game->player_is_ready(username);
    if ( ready )
    {
      std::string notify = ServerMessageMaker::make_notify_prep_phase_done_message().to_string();
      current_game->send_all(notify);
    }
    current_game->unlock_game();
    //TODO. If both players are done with prep_phase send to everyone
  }
  else if (type == ClientRequestValues::REGISTER_USER)
  {
    JSONObject message_value = dynamic_cast< JSONObject & >(*client_message["message_value"]);
    std::string user = JSONString::extract_string(message_value["user"]);
    std::string password = JSONString::extract_string(message_value["password"]);
    bool is_registered = BattleshipsDataSystem::register_user(user, password);
    std::string response = ServerMessageMaker::make_register_response_message(is_registered).to_string();
    user_ptr->set_username(user);
    std::cout << response;
    user_ptr->send_message(response);
  }
  else if (type == ClientRequestValues::RESPONSE_REEF_CONFIG)
  {
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    current_game->lock_game();
    bool accepted = static_cast< bool >(atoi(JSONString::extract_string(client_message["message_value"]).c_str()));
    CurrentGame::ReefDecision desc = current_game->add_user_decision(username, accepted);
    if (desc != CurrentGame::ReefDecision::UNDECIDED)
    {
      std::string response = "";
      if (desc == CurrentGame::ReefDecision::ACCEPTED)
      {
        response = ServerMessageMaker::make_notify_reef_config_message(true, std::vector<Reef>()).to_string();
      }
      else
      {
        std::vector<Reef> reefs = current_game->get_game_state()->set_new_reef_config();
        response = ServerMessageMaker::make_notify_reef_config_message(false, reefs).to_string();
        current_game->reset_reef_decision();
      }
      current_game->send_all(response);
    }
    current_game->unlock_game();
    //TODO: Either send that reef was refused + reef config. Or send accepted.
  }
  else if (type == ClientRequestValues::SAVE_GAME)
  {
    std::string username = user_ptr->get_username();
    auto current_game = active_games.at(user_to_game_id.at(username));
    current_game->lock_game();
    int game_id = current_game->get_game_id();
    std::string name = JSONString::extract_string(client_message["message_value"]).c_str();
    Player::Side turn_of = current_game->get_turn_of();
    auto game_state = current_game->get_game_state();
    bool saved = BattleshipsDataSystem::save_game(game_id, name, turn_of, *game_state);
    std::string response = ServerMessageMaker::make_save_game_response_message(saved).to_string();
    user_ptr->send_message(response);
    current_game->unlock_game();
    //TODO: Save current game. Call save_game on BattleshipsDataSystem.
  }
  else if (type == ClientRequestValues::QUIT_GAME)
  {
    user_quit_game(user_ptr->get_username());
    //TODO: QUIT game. if player quit then close game, else if spectator remove spectator from spectator list 
  }
};

Server::~Server()
{
  SDLNet_Quit();
};
