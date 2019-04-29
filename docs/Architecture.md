Architecture
==========

# Concerns #
This section will lay out the broad strokes of the rest of the document; the
separation of concerns here helps dictate the structure of the rest of the
project.

- Game board
    + Contains
        * Landscape tiles
        * Ships
        * Player bases
- Tile
    + Contains
        * Location
        * Landscape type
- Player
    + Contains
        * Ships
        * Base
        * Score? (Optional added feature)
- Ship
    + Contains
        * Name
        * Size
        * Speed (maximum, effective)
        * Turning speed (in 90° increments)
        * Centre of rotation (from front of boat)
        * Location
        * Orientation
        * Radar/sonar range
        * Has sonar (boolean)
        * Weapon(s?) (Weapons are complicated, suggests an additional class needed.)
        * Armour and damage information (per tile)
    + Provides
        * Menu options (movement, attack, etc.)
        * Moves
        * Attack (per-weapon)
        * Special options depending on equipment
- Weapon
    + Contains
        * Name
        * Range
        * Power
        * Area of Effect (Torpedo and Mine have splash damage) (Mine seems to
          be a special case of weapon as it can have a location)
    + Provides
        * Hit computation (As different weapons provide different hit mechanics)
- Base
    + Contains
        * Damage state per tile
        * Destruction state (boolean)
    + Provides
        * Repair capability for player's ships (if not destroyed)
- Game
    + Contains
        * Game board
        * Current turn
        * Player information
    + Provides
        * Changes of turn
        * Serialization of all game data
        * Reconstruction of game from serialized form
        * Message transfer for hit/miss, etc. from weapons
- Wrapping application
    + Contains
        * Current game
    + Provides
        * Networking access for game
        * Loading/saving infrastructure for serialization of game

This list is incomplete, especially as some components of the project are not
quite clear (or require more thought.)  These items include:

- Action Points (Several actions per player per turn, eg. use more than one ship per turn)
- Object collision
- More later!

Options for later improvement:

- Replay mode (store all events, game state in a serial format for playback)
- Multiplayer (many players controlling a subset of the ships on the map, eg 2v2 with each player controlling half the ships on a team)
