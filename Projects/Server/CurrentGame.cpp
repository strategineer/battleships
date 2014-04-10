#include "CurrentGame.h"
#include <iostream>
#include "BattleshipsDataSystem.h"
#include "ServerMessageMaker.h"
CurrentGame::CurrentGame()
:state(new GameState())
{
  state->set_initial_state();
  state->set_new_reef_config();
  player1_decision = ReefDecision::UNDECIDED;
  player2_decision = ReefDecision::UNDECIDED;
  game_id = BattleshipsDataSystem::create_new_game("", "");
  turn_of = Player::Side::PLAYER_ONE;
  player1_ready_to_start = false;
  player2_ready_to_start = false;
}

CurrentGame::CurrentGame(int id, std::shared_ptr<GameState>& game_state) :
state(game_state)
{
  game_id = id;
  // Do we assume that the players are decided?
  // 
};

void CurrentGame::toggle_turn()
{
  if ( turn_of == Player::Side::PLAYER_ONE )
  {
    turn_of = Player::Side::PLAYER_TWO;
  }
  else
  {
    turn_of = Player::Side::PLAYER_ONE;
  }
}

CurrentGame::~CurrentGame()
{


}

void CurrentGame::reset_reef_decision()
{
  player1_decision = UNDECIDED;
  player2_decision = UNDECIDED;
}

int CurrentGame::get_game_id()
{
  return game_id;
}

Player::Side CurrentGame::get_turn_of()
{
  return turn_of;
}

GameInfo CurrentGame::get_game_info()
{
  GameInfo game_info;
  int num_players = 0;
  if ( player1 != nullptr )
  {
    num_players++;
  }
  if ( player2 != nullptr )
  {
    num_players++;
  }
  game_info.num_players_in_game = num_players;
  game_info.created_by = player1->get_username();
  game_info.max_players_in_game = 2;
  game_info.game_id = game_id;
  std::pair<int, int> wins_and_losses = BattleshipsDataSystem::get_wins_and_losses_of(player1->get_username());
  game_info.creator_wins = wins_and_losses.first;
  game_info.creator_losses = wins_and_losses.second;
  return game_info;
}

Player::Side CurrentGame::add_user(std::shared_ptr<ServerUser> user)
{
  Player::Side user_type = Player::Side::SPECTATOR;
  if ( player1 == nullptr )
  {
    player1 = user;
    user_type = Player::Side::PLAYER_ONE;
    BattleshipsDataSystem::add_player(game_id, 1, user->get_username());
  }
  else if ( player2 == nullptr )
  {
    player2 = user;
    user_type = Player::Side::PLAYER_TWO;
    BattleshipsDataSystem::add_player(game_id, 2, user->get_username());
  }
  else
  {
    spectators.push_back(user);
  }
  return user_type;
}

std::shared_ptr<GameState> CurrentGame::get_game_state()
{
  return state;
}

std::pair<bool, Player::Side> CurrentGame::game_is_won() const
{
  Player::Side ps;
  return std::pair<bool, Player::Side >(state->is_won(ps), ps);
}

void CurrentGame::lock_game()
{
  game_mutex.lock();
}

void CurrentGame::unlock_game()
{
  game_mutex.unlock();
}

void CurrentGame::apply_to_state(std::shared_ptr<ServerUser> user, JSONObject object)
{
  if ( JSONString::extract_string(object["type"]) == "struct" && JSONString::extract_string(object["name"]) == "Command" )
  {
    state->deliver_command(Command::unserialize(object));
  }
}

void CurrentGame::send_all_except_me(std::shared_ptr<ServerUser> me, std::string message)
{
  if ( me != player1 && player1 != nullptr )
  {
    ( *player1 ).send_message(message);
  }
  if ( me != player2 && player2 != nullptr )
  {
    ( *player2 ).send_message(message);
  }
  for ( auto it = spectators.begin(); it != spectators.end(); it++ )
  {
    if ( ( *it ) != me && ( *it ) != nullptr )
    {
      ( **it ).send_message(message);
    }
  }

}

void CurrentGame::send_all(std::string message)
{
  if ( player1 != nullptr )
  {
    ( *player1 ).send_message(message);
  }
  if ( player2 != nullptr )
  {
    ( *player2 ).send_message(message);
  }
  for ( auto it = spectators.begin(); it != spectators.end(); it++ )
  {
    if ( ( *it ) != nullptr )
    {
      ( **it ).send_message(message);
    }
  }
}

CurrentGame::ReefDecision CurrentGame::add_user_decision(std::string username, bool accepted)
{
  ReefDecision agreed_decision = ReefDecision::UNDECIDED;
  ReefDecision player_decision = ReefDecision::UNDECIDED;
  if ( accepted )
  {
    player_decision = ReefDecision::ACCEPTED;
  }
  else
  {
    player_decision = ReefDecision::REFUSED;
  }
  if ( player1 != nullptr && player1->get_username() == username )
  {
    player1_decision = player_decision;
  }
  else if ( player2 != nullptr && player2->get_username() == username )
  {
    player2_decision = player_decision;
  }
  if ( player1_decision == ReefDecision::UNDECIDED
      || player2_decision == ReefDecision::UNDECIDED )
  {
    return ReefDecision::UNDECIDED;
  }
  else if ( player1_decision == ReefDecision::ACCEPTED
           && player2_decision == ReefDecision::ACCEPTED )
  {
    return ReefDecision::ACCEPTED;
  }
  return ReefDecision::REFUSED;
}

std::string CurrentGame::get_player_one()
{
  std::string player = "";
  if ( player1 != nullptr )
  {
    player = player1->get_username();
  }
  return player;
}

std::string CurrentGame::get_player_two()
{
  std::string player = "";
  if ( player2 != nullptr )
  {
    player = player2->get_username();
  }
  return player;

}

void CurrentGame::load_game(LoadedGame loaded_game)
{
  if ( loaded_game.success )
  {
    game_id = loaded_game.new_game_id;
    state = loaded_game.loaded_gs;
    turn_of = loaded_game.current_turn_of;
    if ( loaded_game.player_one != player1->get_username() )
    {
      auto temp_user_ptr = player2;
      player2 = player1;
      player1 = temp_user_ptr;
    }
    std::string player_one_notify = ServerMessageMaker::make_notify_load_game_message(turn_of, Player::Side::PLAYER_ONE, state).to_string();
    player1->send_message(player_one_notify);
    std::string player_two_notify = ServerMessageMaker::make_notify_load_game_message(turn_of, Player::Side::PLAYER_TWO, state).to_string();
    player2->send_message(player_two_notify);
    std::string spectator_notify = ServerMessageMaker::make_notify_load_game_message(turn_of, Player::Side::SPECTATOR, state).to_string();
    for ( auto it = spectators.begin(); it != spectators.end(); it++ )
    {
      ( *it )->send_message(spectator_notify);
    }
  }
}

std::vector<std::string> CurrentGame::get_spectators()
{
  std::vector<std::string> s_spectators;
  for ( auto it = spectators.begin(); it != spectators.end(); it++ )
  {
    s_spectators.push_back(( *it )->get_username());
  }
  return s_spectators;
}

bool CurrentGame::player_is_ready(std::string username)
{
  if ( player1->get_username() == username )
  {
    player1_ready_to_start = true;
  }
  else if ( player2->get_username() == username )
  {
    player2_ready_to_start = true;
  }
  if ( player1_ready_to_start && player2_ready_to_start )
  {
    return true;
  }
  return false;
}

bool CurrentGame::is_player(std::string username)
{
  if ( player1->get_username() == username )
  {
    return true;
  }
  if ( player2->get_username() == username )
  {
    return true;
  }
  return false;
}

void CurrentGame::remove_spectator(std::string username)
{
  for ( auto it = spectators.begin(); it != spectators.end(); it++ )
  {
    if ( ( *it )->get_username() == username )
    {
      spectators.erase(it);
      return;
    }
  }
}