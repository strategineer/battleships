#pragma once
#include <memory>
#include <utility>
#include <array>
#include "Boat.h"
#include "Command.h"
#include "Player.h"
#include "ICollisionDetector.h"
#include "IMineHandler.h"
#include "ICollidable.h"
#include "Mine.h"
#include "RadarBoat.h"
#include "Reef.h"
#include <vector>

class GameState;

struct PlayerData
{
  //Player user;
  std::vector<std::shared_ptr<Unit> > units;
  std::shared_ptr<Base> home_base;
  RgbColor color;

  JSONObject serialize() const;
  static PlayerData unserialize(JSONObject object, GameState& state);
};

class GameState :
  public ICollisionDetector, public IMineHandler, Serializable
{
private:
  unsigned int game_id;
  PlayerData player_one, player_two;
  std::vector<std::shared_ptr<Reef>> reefs;
  std::vector<std::shared_ptr<Mine>> mines;
  // Note that this does not adequately reflect all possible obstacles, prefer another abstraction.
  // Currently mines aren't listed at all.

public:
  GameState(); //init
  GameState(JSONObject object);
  ~GameState();
  // GameState(Player one, Player two); //load
  void set_initial_state();

  CommandResult deliver_command(const Command& cmd);

  virtual std::shared_ptr<ICollidable> collision_check(const Point& pt) override;
  std::vector<Boat> get_players_boats(Player::Side player);
  std::pair<Player::Side, Boat> get_boat_at(Point position);
  std::array<std::array<Radar::Level, 30>, 30> get_visibility(Player::Side side) const;
  CommandResult swap_boats(int boat_id, Point position);
  const PlayerData & get_player_one() const;
  const PlayerData & get_player_two() const;
  std::vector<Reef> get_reefs() const;
  std::vector<Mine> get_mines() const;
  std::vector<Reef> set_new_reef_config();
  void set_reef_config(std::vector<Reef> preefs);
  bool GameState::is_mine_at(const Point& p);

  bool is_won(Player::Side& player) const;

  virtual JSONObject serialize() const;

  virtual bool add_mine(const Mine& mine);
  virtual bool remove_mine(const Point& mine_at);
  virtual std::shared_ptr<Unit> get_unit_by_id(int pid);

};