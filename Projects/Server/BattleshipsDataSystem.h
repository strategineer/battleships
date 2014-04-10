#pragma once
#include <string>
#include "sqlite3.h"
#include "GameState.h"
#include <exception>
#include "Player.h"
#include <mutex>
#include "LoadedGame.h"
#include "SavedGame.h"

class UserAlreadyExistsException : public std::exception
{
  virtual const char* what() const throw( )
  {
    return "User already exists";
  }

};

class UnableToOpenDatabase : public std::exception
{
  virtual const char* what() const throw( )
  {
    return "Unable to open database";
  }

};

class BattleshipsDataSystem
{
private:
  static sqlite3* database;
  static int current_game_id;
  static std::mutex id_mutex;
public:
  static void init_database();
  static void close_database();
  static bool authenticate_user(std::string user, std::string password);
  static bool register_user(std::string user, std::string password);
  static int create_new_game(std::string player_one, std::string player_two);
  static bool save_game(int game_id, std::string name, Player::Side turn_of, GameState& state);
  static void record_completed_game(int game_id, Player::Side winner);
  static std::vector<SavedGame> get_saved_games_of(std::string user_one, std::string user_two);
  static LoadedGame load_game(int save_game_id);
  static bool add_player(int game_id, int player_number, std::string player);
  static std::pair<int, int> get_wins_and_losses_of(std::string user);
};