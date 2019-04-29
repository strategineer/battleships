Serialization format
==============

# Basic structure #

I've considered two approaches to a serialized format for the game; one is
JSON, the other, a custom binary format.  For most cases binary seems like the
best choice, as string parsing into objects in C++ is a painful, painful thing.

# Initial structure #

The first few bytes of the file should be an identifier so that attempting to
load a random file doesn't generate a more difficult exception.  Followed by
that will be a version code (either two or four-byte array of uint8s, with a
build date after that.)  If the version code is less than or equal to the
defined version code, then the loading can proceed; build date will be
included in case I use that in the future.

The game will be identified by a GUID immediately after that structure, and
both players' copies will have the same GUID.  Following that is a uint8 that
stores the player number (note: limit of 256 players.  Maybe 255 if 0 is an
observer.)

# Map layout #

Each tile on the map is at most coral or water; this suggests that there is
very little information required to draw the map (ignoring units that may be
on top of it), and that in turn suggests only a very basic design for the
map.  While otherwise I would consider using a full byte (or more) for each
tile, since so little data is expected to be needed, a nybble will suffice. 
As such, each row of the grid will require an array of 15 bytes, with the low
nybble of `serial_map[x][y]` referring to `map[x/2][y]`, and the high nybble 
`map[x/2 + 1][y]`.  `x` and `y` here would be in the ranges `[0 .. 14]` and 
`[0 .. 29]` respectively.

# Player data #

The two options I've considered have had each player holding a copy of the 
data, or only one player holding the game, and the other player loading the
data over the network; in the first case both players have a copy of the game
data, in the second one has to receive the game data and then construct their
in-memory game.  I'm leaning towards the first method simply because it
reduces the number of construction methods required; it also requires both
players loading their game data initially and then comparing the provided
check values.  This will, however, make game loading slightly more complicated
from one perspective.

The first value in the player data is which player this data represents.  This
will be a `uint8` since there's not supposed to be that many players.

After that will be an array of five `uint8`s representing the player's base's
damage status; each nybble of it represents the base's status from bottom to
top, little-endian style.  Given that the base is considered "heavily
armoured", this means that the range for each nybble is between 0 and 2; 0,
it's destroyed, 2, it's fully-reinforced.  Most likely the base will have a
constructor that takes an array of expanded `uint8`s, not the unexpanded
nybbles---for future-proofing.

# Ship data #

There's a fixed number of types of ships, so the first byte of each ship
structure can store the type of ship--defining the number of cells in both its
armour and its current health arrays, as well as defining which sets of bow
sprites it loads. After the definition of the ship type, then, there is a uint8
for its maximum speed, two bytes to define its location on the map (from the
stern), a byte to define its direction (0: north, 1: east, 2: south, 3: west),
and -- (stopping here since the ship needs to be better described before I can
finalise the serialization.)