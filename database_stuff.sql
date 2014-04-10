/* Create users table. Password are stored as plain text.*/

CREATE TABLE users(username varchar(30) not null, password varchar(30) not null, primary key(username));
 
 
/* Create games table. If winner set to 0 this means the game has not ended yet. If 1 player1 won, else if 2 player2 one*/

CREATE TABLE games(id unsigned big int not null, player1 varchar(30), player2 varchar(40), winner int DEFAULT 0, foreign key(player1) references users(username)
, foreign key(player2) references users(username), Constraint check_winner CHECK(winner in (0,1,2)));


/* Create saved games table */

CREATE TABLE saved_games(game_id unsigned big int, save_id unsigned int, serialized_game varchar(100000), primary key(game_id, save_id)
, foreign key(game_id) references games(id));


/* Get all saved games (that hasn't ended yet) of a pair of user. Replace player, other_player by usernames of an actual users. */

SELECT game_id, save_id FROM saved_games SG, games G WHERE SG.game_id = G.id 
                                                           and ((G.player1 = "player" and G.player2 = "other_player") or (G.player1 = "other_player" and G.player2  = "player"))
                                                     ORDER BY save_id DESC;
                                                     
(SELECT "Won", count(*) as game_result FROM Games WHERE (G.player1 = "player" and G.winner=1) or (G.player2  = "player" and G.winner=2))
 
