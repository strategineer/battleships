Class layout
===========

# Broad divisions #
This layout of classes divides the classes into game-specific and
application-general classes, with additional information for the required
state of each.  Some information is currently unavailable as graphics and
event/message handling have not been sorted out yet, but will be noted with
fields, etc. where necessary.  Those details will become more complete as
details are developed.

# Game-state classes #

## Game ##
A singleton which can only be created or destroyed by the containing application
(as both its constructor and destructor are protected and the container is a
friend class.)  The rest of the game is directed through events and messages
that are directed into the game and propagated through the local state (and vice
versa.)

## Player ##

## Unit ##
This class should never be directly instantiated, as it only defines a few
basic interactions; it may be entirely virtual (haven't decided just yet, that
can wait until implementation.)  It provides potected attributes 
`uint8 maximum_health[], current_health[]`, as well as defining a show menu
method, as well as health-altering methods.

###### Fields ######

- `uint8 maximum_health`: defines the maximum health of each cell of the unit;
currently it's just one value since the whole unit has only one level of
armouring (2: heavy, 1: normal)
- `std::vector<uint8> current_health`: sets the current health of each cell of
the unit, indexed from the front as `0`.  Note that this cannot go higher than
2 and cannot go lower than 0

###### Methods ######

- `virtual void show_menu()`: Overridden by all inheriting classes, only
  presents "Cancel".
- `virtual void modify_health(int8 delta)`: changes the health by provided
  delta; does bounds-checking to prevent over/underflows and throws a range
  exception if the provided value would cause such an error.  (Called by Base
  and all Weapon derivatives.)
- `bool is_destroyed()`: returns true if the unit is destroyed else false.

### Base ###
The player's base inherits from Unit; it provdes a function to repair a ship
(assuming required conditions are met), and overrides the menu-showing method
in Unit.  It also stores the necessary sprites for all its conditions.

###### Fields ######

- Once graphics are figured out, fields for the various 

###### Methods ######

- `void show_menu() override`: Overrides `Unit`, and depending on its damage
  state offers certain options.
- `void repair_damage (Ship *vessel)`: If a vessel is occupying a tile adjacent
  to the base, and the base is not destroyed, repair one unit of damage on the
  ship.  Throws an exception if any of those conditions are not met and it is
  somehow called.  (Does not offer to repair fully-restored ship?)

### Ship ###
Similarly to Base, Ship inherits its basic health-monitoring mechanics from
Unit, and likewise overrides its menu-showing method, building the list of
options based on the weapons it has equipped.  (Weapons will be stored in a
vector, since they aren't going to be changing position very often after
they're created, and it provides useful methods.)

###### Fields #######

- `std::string name`: Name of the ship's class
- `uint8 size`: Length of the ship
- `uint8 maximum_speed`: Speed of the ship when not damaged
- `uint8 current_speed`: Speed of the ship in current damage state; decreases
  by one for each unit of damage taken.
- `uint8 centre_of_rotation`: Cell within the ship around which rotation
  occurs; must be less than the length of the ship (indexed from 0 for bow of
  vessel.) [Do not require both turning radius and centre of rotation, as the
  second describes the first.]
- `uint8 speed_of_rotation`: Either 1 or 2; defines the rotation speed in 90°
  increments.
- `Point location`: Defines the location of the vessel from the bow in x-y
  coordinates (probably using a Point wrapper class or struct.)
- `Direction orientation`: Defines the orientation of the vessel with respect to the four cardinal directions (North, East, South, West).
- `??? radar_range`: Defines how far the vessel's radar reaches in in each direction (can be defined by a rectangle?). Should Radar be its own class? That would be better I think so that we could refactor the radar range, sonar from the Ship class and put it into its own class which Ship and Base would aggregate (remember the Base needs to have a radar as well with 1 square range).
- `bool sonar`: Defines if the ship has sonar or not; for all but the mine-ship
  this is `false`.
- `std::vector<Weapon> weapon_list`: Equipped weapons; currently unsure if
  Weapon should be a class or an interface, as each weapon reacts in a somewhat
  different manner to being informed that it should fire.  Probably will be a
  class that is inherited by Torpedo, Mine-Layer and Mine, since default Weapon
  behaviour will be effective for Cannon and Heavy Cannon.

## Weapon ##
The weapon class will be sufficient for a cannon (or heavy cannon), however
torpedos mines, and mine-laying will inherit from Weapon, as they require
different hit computation methods than the simple hit-or-miss form used
by cannons.  (However, they don't require their own sections here just yet,
since torpedo *only* overrides hit computation, similar to mines.)

## Tile ##
Tiles will be flyweight objects, containing just a location and a pointer to a
static object which stores the image for the tile, as well as its properties
(can be traversed, etc.)  Those tiles will probably be called TileBase or
something similar.

# Application-level classes #
Outside of the game, various additional tools are necessary, such as an
interface to load a game and to save it to file, to negotiate the network
connection, and to interact with the OS and display the game.

## Serialization ##
Constructing a serializer should attempt to open an output file, and by
providing a string representing the location the game should be written to,
should open the location and convert the structure of the game to its serial
form; to aid in that, each game structure will implement a "serialize" method
(in fact, an ISerializable interface).

