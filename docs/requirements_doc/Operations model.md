Operations Model
================

Operation: Client::Quit()
Scope: Client
Messages: Client::{playerDisconnected}
Description: Exits the application, dropping any unissued modifications to game
state.

Operation: Server::StartGame(room\_id: int)
Scope: Client; Server; User; Game; Room
Messages: Client::{initialGame};
Description: Generate a new game with the users in the room as players.

Operation: Server::JoinGame(game\_id: int)
Scope: Client; Server; User; Game
Messages: Server::{loadedGame};
Description: Restore a previously saved game in which the user is one of the
players.

Operation: Server::SaveGame(gameId: int)
Scope: Server, Game
Messages: Client::{saveGameAnswer};
Description: Server records game state at the current point in play (or failed
to) and notifies the player.

Operation: Server::Login(username: string, password: string)
Scope: Client; Account
Messages: Client::{loginResult};
Description: User requests access to account with credentials provided; if the
data is valid, the server responds `true` and with further account details,
else the user needs to try and log in again.

Operation: Server::register(username: string, password: string)
Scope: Client; Account
Messages: Client::{registerResult}
Description: User requests the creation of a new account with details as
provided; if the account details are valid (i.e. no existing account with that
name) a new account is created. Otherwise, the user needs to try again.

Operation: Server::JoinRoom(roomId:int, user:User)
Scope: Room; User
Messages: Client::{room};
Description: Join an existing room before creating a game.

Operation: Server::createRoom(user: User)
Scope: Room; User
New: Room
Messages: Client::{room};
Description: Create a room run by the provided user.

Operation: Game::reefDecision(verify: Boolean)
Scope: Game
Messages: Game::{reefConfiguration};
Description: Notifies the server of the user's decision about the validity of
the reef.  If players concur, the game starts, else a new reefConfiguration is
sent.

Operation: Game::SwapBoats(boat1: Boat, boat2: Boat)
Scope: Game; User; Boat
Messages: Game::{swapBoats}
Description: Notifies the server that the user changed the location of two
boats at the start of the game.

Operation: Game::SendMessage(message: String)
Scope: Game
Messages: Client::{receiveMessage};
Description: User sends a message to the other player, it is recorded for
future playback.

Operation: Game::TakeTurn(command: Command, tile: Tile)
Scope: Game, Boat, Tile, Command
Messages: Game::{yourTurn; boatCommandResult; spectatedGameEvent};
Description: An action is performed on a boat (possibly involving weapons or 
sensing equipment), and a result is generated (can be local, can be on the
server.)  The `spectatedGameEvent` is sent to any currently-attached
spectators.

Operation: Client::WatchGame(gameId: int)
Scope: Game; Client; Server
Messages: Server::{spectatedGame};
Description: Request all GameEvents that are logged for a specific game, in
order to watch it after the fact (or during play, perhaps with a delay.)
