#include "BattleshipsDataSystem.h"
#include <iostream>
#include <time.h>

int BattleshipsDataSystem::current_game_id = 0;
sqlite3 *BattleshipsDataSystem::database = NULL;
std::mutex BattleshipsDataSystem::id_mutex;

void BattleshipsDataSystem::init_database()
{
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("battleships.db", &database);

  if ( rc )
  {
    throw UnableToOpenDatabase();
  }
  else
  {
    fprintf(stderr, "Opened database successfully\n");
  }
  std::string sql_user_table = "CREATE TABLE USERS(" \
    "username TEXT PRIMARY KEY NOT NULL," \
    "password TEXT NOT NULL);";
  std::string sql_games_table = "CREATE TABLE GAMES(" \
    "game_id INT PRIMARY KEY NOT NULL," \
    "player_one TEXT," \
    "player_two TEXT," \
    "winner int DEFAULT -1," \
    "FOREIGN KEY(player_one) REFERENCES USERS(username)," \
    "FOREIGN KEY(player_two) REFERENCES USERS(username)," \
    "CONSTRAINT check_winner CHECK(winner in (-1, 0, 1)));";
  std::string sql_saved_games_table = "CREATE TABLE SAVED_GAMES(" \
    "saved_game_id INTEGER PRIMARY KEY NOT NULL," \
    "game_id INT NOT NULL," \
    "name TEXT," \
    "date TEXT NOT NULL," \
    "turn_of INT NOT NULL," \
    "serialized_game_state TEXT NOT NULL," \
    "FOREIGN KEY(game_id) REFERENCES GAMES(game_id)," \
    "CONSTRAINT check_turn CHECK(turn_of in (0,1)));";

  //create users table
  sqlite3_stmt * stmt_user;
  sqlite3_prepare_v2(database, sql_user_table.c_str(), -1, &stmt_user, NULL);
  int s = sqlite3_step(stmt_user);
  if ( s == SQLITE_DONE )
  {
    std::cout << "Created new USERS table" << std::endl;
  }
  else
  {
    std::cout << "USERS table already exists" << std::endl;
  }

  //create games table
  sqlite3_stmt * stmt_game;
  sqlite3_prepare_v2(database, sql_games_table.c_str(), -1, &stmt_game, NULL);
  s = sqlite3_step(stmt_game);
  if ( s == SQLITE_DONE )
  {
    std::cout << "Created new GAMES table" << std::endl;
  }
  else
  {
    std::cout << "GAMES table already exists" << std::endl;
    std::string sql = "SELECT max(game_id) FROM Games;";
    sqlite3_stmt * stmt;
    sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    int s = sqlite3_step(stmt);
    if ( s == SQLITE_ROW )
    {
      current_game_id = sqlite3_column_int(stmt, 0) + 1;
    }
  }

  //create saved games table
  sqlite3_stmt * stmt_saved;
  sqlite3_prepare_v2(database, sql_saved_games_table.c_str(), -1, &stmt_saved, NULL);
  s = sqlite3_step(stmt_saved);
  if ( s == SQLITE_DONE )
  {
    std::cout << "Created new SAVED_GAMES table" << std::endl;
  }
  else
  {
    std::cout << "SAVED_GAMES table already exists" << std::endl;
  }
}

void BattleshipsDataSystem::close_database()
{
  sqlite3_close(database);
}

bool BattleshipsDataSystem::authenticate_user(std::string user, std::string password)
{
  std::string sql = "SELECT password FROM Users WHERE username='" + user + "';";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_ROW )
  {
    const unsigned char *pass_attribute;
    pass_attribute = sqlite3_column_text(stmt, 0);
    if ( strcmp(password.c_str(), (const char*)pass_attribute) == 0 )
    {
      return true;
    }
  }
  return false;
}

bool BattleshipsDataSystem::register_user(std::string user, std::string password)
{
  std::string sql = "INSERT INTO Users values('" + user + "', " + "'" + password + "');";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_DONE )
  {
    std::cout << "New user has registered: " << user << std::endl;
    return true;
  }
  else if ( s == SQLITE_CONSTRAINT )
  {
    return false;
  }
  return false;
}

//SHOULD ONLY BE CALLED WHEN BOTH USERS ARE CONNECTED TO A GAME
int BattleshipsDataSystem::create_new_game(std::string player_one, std::string player_two)
{
  //Ensures that anyone trying to get a new game_id will have a different one
  id_mutex.lock();
  int id = current_game_id++;
  id_mutex.unlock();

  std::string sql = "INSERT INTO Games(game_id, player_one, player_two) values(" + std::to_string(id) + ", '" + player_one + "', '" + player_two + "');";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_DONE )
  {
    std::cout << "New game created: " << id << std::endl;
    return id;
  }
  return -1;
}

bool BattleshipsDataSystem::add_player(int game_id, int player_number, std::string player)
{
  std::string sql = "SELECT player_two FROM Games WHERE game_id=" + std::to_string(game_id) + ";";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_ROW )
  {
    std::string player_two = std::string(( const char * ) sqlite3_column_text(stmt, 0));
    if ( player_two != "" )
    {
      return false;
    }
    std::string sql = "";
    if ( player_number == 1 )
    {
      sql = "UPDATE Games SET player_one='" + player + "' WHERE game_id=" + std::to_string(game_id) + +";";
    }
    else
    {
      sql = "UPDATE Games SET player_two='" + player + "' WHERE game_id=" + std::to_string(game_id) + +";";
    }
    sqlite3_stmt * stmt;
    sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
    int s = sqlite3_step(stmt);
    if ( s == SQLITE_DONE )
    {
      return true;
    }
  }
  return false;
}


bool BattleshipsDataSystem::save_game(int game_id, std::string name, Player::Side turn_of, GameState& state)
{
  //Get current date
  time_t now = time(0);
  struct tm  tstruct;
  char date[80];
  tstruct = *localtime(&now);
  strftime(date, sizeof( date ), "%Y-%m-%d.%X", &tstruct);
  std::string sdate(date);

  std::string sql = "INSERT INTO Saved_Games(game_id, name, date, turn_of, serialized_game_state)" \
    "values(" + std::to_string(game_id) + ", '" + name + "', '" + sdate + "', " + std::to_string(static_cast<int>(turn_of)) + ", '" + state.serialize().to_string() + "');";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_DONE )
  {
    std::cout << "Saved Game #" << game_id << " name=" << name << " date=" << sdate << std::endl;
    return true;
  }
  else
  {
    std::cout << "Error saving game..." << std::endl;
  }
  return false;
}

void BattleshipsDataSystem::record_completed_game(int game_id, Player::Side winner)
{
  std::string sql = "UPDATE Games SET winner = " + std::to_string(winner) + " WHERE game_id = " + std::to_string(game_id) + "; ";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_DONE )
  {
    std::cout << "Player " << winner + 1 << " won game #" << game_id << std::endl;
  }
  else if ( s == SQLITE_CONSTRAINT )
  {
    std::cout << "Invalid winner (" << winner + 1 << ") won game #" << game_id << std::endl;
  }
  else
  {
    std::cout << "Error: recording completed game #" << game_id << std::endl;
  }
}

std::vector<SavedGame> BattleshipsDataSystem::get_saved_games_of(std::string user_one, std::string user_two)
{
  std::vector<SavedGame> saved_games;
  std::string sql = "SELECT saved_game_id, name, date FROM saved_games SG, games G WHERE SG.game_id = G.game_id " \
    "and ((G.player_one ='" + user_one + "' and G.player_two = '" + user_two + "') " \
    "or (G.player_one = '" + user_two + "' and G.player_two  = '" + user_one + "')) " \
    "ORDER BY saved_game_id DESC;";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  while ( 1 )
  {
    int s = sqlite3_step(stmt);
    if ( s == SQLITE_ROW )
    {
      SavedGame saved_game;
      saved_game.saved_game_id = sqlite3_column_int(stmt, 0);
      saved_game.name = std::string(( const char * ) sqlite3_column_text(stmt, 1));
      saved_game.timestamp = std::string(( const char * ) sqlite3_column_text(stmt, 2));
      saved_games.push_back(saved_game);
    }
    else
    {
      break;
    }
  }
  return saved_games;
}

std::pair<int, int> BattleshipsDataSystem::get_wins_and_losses_of(std::string user)
{
  int wins = 0;
  int losses = 0;
  std::string sql_wins = "SELECT count(*) as wins FROM Games WHERE (player_one = '" + user + "' and winner=0) or (player_two  = '" + user + "' and winner=1);";
  std::string sql_losses = "SELECT count(*) as losses FROM Games WHERE (player_one = '" + user + "' and winner=1) or (player_two  = '" + user + "' and winner=0);";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql_wins.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_ROW )
  {
    wins = sqlite3_column_int(stmt, 0);
  }
  sqlite3_prepare_v2(database, sql_losses.c_str(), -1, &stmt, NULL);
  s = sqlite3_step(stmt);
  if ( s == SQLITE_ROW )
  {
    losses = sqlite3_column_int(stmt, 0);
  }
  return std::pair<int, int>(wins, losses);
}

LoadedGame BattleshipsDataSystem::load_game(int save_game_id)
{
  LoadedGame loaded_game;
  loaded_game.success = false;
  std::string sql = "SELECT SG.turn_of, SG.serialized_game_state, G.player_one, G.player_two FROM Saved_Games SG, Games G WHERE SG.saved_game_id=" + std::to_string(save_game_id) + " and SG.game_id = G.game_id;";
  sqlite3_stmt * stmt;
  sqlite3_prepare_v2(database, sql.c_str(), -1, &stmt, NULL);
  int s = sqlite3_step(stmt);
  if ( s == SQLITE_ROW )
  {
    loaded_game.current_turn_of = static_cast<Player::Side>(sqlite3_column_int(stmt, 0));
    std::string serialized_game_state = std::string((const char *)sqlite3_column_text(stmt, 1));
    loaded_game.player_one = std::string((const char *)sqlite3_column_text(stmt, 2));
    loaded_game.player_two = std::string((const char *)sqlite3_column_text(stmt, 3));
    loaded_game.new_game_id = create_new_game(loaded_game.player_one, loaded_game.player_two);
    loaded_game.loaded_gs = std::shared_ptr<GameState>(new GameState(JSONObject::parse_object(serialized_game_state)));
    loaded_game.success = true;
    std::cout << "Loaded game with saved_game_id= " << save_game_id << ", loaded game with new game_id=" << loaded_game.new_game_id << std::endl;
  }
  else
  {
    std::cout << "Could not find saved game." << std::endl;
  }
  return loaded_game;
}