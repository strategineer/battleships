NB: All calls can happen directly on Game but once we figure out which classes do what we'll call the methods on their respective classes/controllers.

All/Sub-operations (might need to be given sequence diagrams or will be integrated if used more than once else they'll be integrated into the diagram they're called in:
- isValidAction(Action) : Boolean
- isLocationEmpty(Point) : Boolean
- hasMines(Ship) : Boolean
- getVisibilityFor(Player) : Boolean[][]

Ben: 
- newGame(p1 : Player, p2 : Player)
- positionShip(s : Ship, position : Point)
- moveShip(s : Ship, location : Point)

Franz: 
- turnShip(s : Ship, direction : DirectionEnum)
- fireCannon(s : Ship, location : Point)
- fireTorpedo(s : Ship)

Bill: 
- dropMine(s : Ship, location : Point)
- isVisible(location : Point, p : Player)
