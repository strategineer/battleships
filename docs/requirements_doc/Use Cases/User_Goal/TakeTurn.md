**Use Case**: TakeTurn

**Scope**: BattleShips

**Level**: User Goal

**Intention in Context**: The intention of the _Player_ is to take his/her turn in a match of BattleShips.

**Multiplicity**: One _Player_ in a game may take their turn at a time. Many _Players_ in different games may take their turns  simultaneously.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _System_ shows _Player_ current game state.

        _Player_ observes game state.
2. _Player_ asks _System_ to select a ship.
3. _System_ shows _Player_ available actions for the selected ship.

        _Player_  observes available actions. The available actions may depend on the location of the ship and the type of ship. The available actions may include MoveShip, DeployRadar, RepairShip, DropMine, PickupMine, ShootCannon, ShootTorpedo.
4. _Player_ informs _System_ of their chosen action.

        _System_ performs action on game state.
5. _System_ shows _Player_ resulting game state.

**Extensions**:

- 3a. If the _Player_ selects another ship; continue use case at step 3.
- 5a. If the chosen action triggers any notification messages then the _System_ sends the relevant messages to each _Player_.
