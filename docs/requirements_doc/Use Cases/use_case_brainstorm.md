# Use Cases for guiding UI #
## Setting up ##
1. Launch game.
2. Click on a main menu option to continue.
	- Find Opponent
		1. Observe list of all players (PLAYER_NAME --- WINS/GAMES_PLAYED)
		2. Click on name of player you'd like to play against.
		3. The other player will receive an invitation with your name and stats, and can decide whether to play with you or not.
		4. If the other player does not accept return to step 1, else the game starts.
	- Options
		- Set your options: your display name, favorite ship color?, volume, ... 
	- Quit
		- Close the game and return to the desktop.

## Game Phases ##
### Reef Setup ###
1. Observe generated reef configuration.
2. If you find it acceptable click on Accept button, else click on refuse button.
3. If both players accept the reef configuration then Ship Setup phase starts, else a new reef is generated and repeat step 1.

### Ship Setup ###
1. Click on a ship to select it.
2. Click on another ship to switch locations with it or click on a base tile without any ship docked to move it there.
3. If finished setting up ships click the Done button, else repeat from 1.
4. When both players have finished their ship setup then Coin Toss phase starts.

### Coin Toss ###
1. One, randomly chosen, player chooses heads or tails.
2. Observe the coin toss.
3. If you win the coin toss then choose whether you'd like to start first or not by clicking on the first or second buttons, wait for the other player to choose.
4. The Player Turn phase starts.

### Player Turn ###
1. Hover over ship with mouse cursor.
    - Ship specific info will be displayed on one of the bottom corners of the screen.
2. Wait for dropdown menu listing all available actions to appear.
3. Hover over each action/option to obtain more info about the action.
    - Move -> Show move "range" on map (any tiles where the ship can end up should be highlighted).
    - Shoot Cannon -> Show cannon range on map.
    - Shoot Torpedo -> Show expected torpedo path on map (max 10 units from front of ship until hits ship or coral).
    - Drop Mine -> Show remaining mines and show valid locations for dropping mines on map.
    - Pickup Mine -> Highlight mines in range for pickup.
    - Repair a ship -> Show ship square that will be repaired.
    - Toggle Radar -> Show new radar range (after toggle) on map.
4. Click on an action to select it.
5. Proceed accordingly depending on the action.
    - Move -> Click on one of the "move select" buttons arranged in a keypad formation around the ship (for showing user valid movements for the current ship type (backward x units, forward x units, turn left, turn right, 180 degree turn, etc.?))
        - Afterwards, maybe you must click a tile to further specify the movement (eg. if "move forward" button is clicked then you must choose the tile to move to specifically).
    - Shoot Cannon -> Click on the tile to shoot at.
    - Shoot Torpedo -> Do nothing (Automatic).
    - Drop Mine -> Click on the tile to place mine on.
    - Pickup Mine -> Click on the mine to pickup.
    - Repair ship -> Do nothing (Automatic).
    - Toggle Radar -> Do nothing (Automatic).
6. [Optional]Repeat ad nauseum for other ships (depending on if player's can move several ships or not).


## Notifications ##
Notifications will appear in one of the top corners of the screen with relevant information displayed on the map depending on the type of message sent. Unless otherwise stated all notifications are sent to both players.
- Shot
    - Location: the coordinates of the impact (Shown graphically on the map, flashing red X marks the spot).
    - Cause: kind of weapon used (cannon, torpedo, or mine).
    - Effect:
        - Nothing / Shot fell into the water
            - if cannon then both players receive the notifications.
            - if torpedo then only the player who shot it receives the notification.
        - Mine exploded
        - Ship hit
            - whether or not the shot damages a square the notification is still displayed. 
        - Base hit
        - Ship sunk
        - Base destroyed
    - Extra Info:
        - If ship sunk, the type of ship is shown to the player who sank it. 
- Movement Collision
    - Location of collision
