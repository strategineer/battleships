**Use Case**: SpectateMatch

**Scope**: BattleShips

**Level**: User Goal

**Intention in Context**: The intention of the _Spectator_ is to spectate a match of BattleShips.

**Multiplicity**: A _Spectator_ may decide to spectate a match. Many _Spectators_ may spectate the same match simultaneously.

**Primary Actor**: _Spectator_

**Main Success Scenario**:

1. _System_ shows _Spectator_ list of matches currently being played.

        _Spectator_ observes list of matches.
2. _Spectator_ informs _System_ of the match he/she wishes to spectate.
3. _System_ shows _Spectator_ loading screen.
4. _System_ adds _Spectator_ to the given match as a spectator.

        _Spectator_ spectates the match.
5. _Spectator_ tells _System_ he/she wishes to leave the match.
6. _System_ disconnects _Spectator_ from the match. 
7. _System_ returns _Spectator_ to the main menu. 

**Extensions**:
- 2a. If _Spectator_ does not wish to spectate any of the matches shown; use case fails.
- 5a. If the match is over; continue use case at step 6.
