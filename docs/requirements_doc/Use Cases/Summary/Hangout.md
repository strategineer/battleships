**Use Case**: Hangout

**Scope**: BattleShips

**Level**: Summary

**Intention in Context**: The intention of one _Player_ is to invite another _Player_ to a room to hangout before a starting a match of BattleShips.

**Multiplicity**: One _Player_ can hangout in a room with one other _Player. Many pairs of _Players_ may hangout simultaneously.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _Player A_ asks _System_ to invite _Player B_ to a room.
2. _System_ shows _Player B_ the invitation.

        _Player B_ observes _Player A_'s BattleShips stats and username.
3. _Player B_ sends confirmation to the _System_.
4. _System_ shows confirmation to _Player A_.
5. _System_ moves _Player A_ and _Player B_ to a room together.
        
        _Players_ hangout.
6. _Player A_ or _Player B_ can decide to: start a new match (_StartNewMatch_) or load a match (_LoadMatch_).
7. _System_ begins the match (_PlayMatch_) between _Player A_ and _Player B_ in the room.

**Extensions**:

- 3a. If _Player B_ refuses the invitation; use case fails.
- 6a. Any _Player_ may send a message to the other _Player_ (_SendMessage_).
- 6b. If neither _Player_ wishes to start a match; continue use case at step 6.
- 6c. One of the _Players_ informs _System_ that they wish to leave the room; use case fails.
