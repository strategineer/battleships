**Use Case**: PlayMatch

**Scope**: BattleShips

**Level**: Summary

**Intention in Context**: The intention of the pair of _Players_ is to play a match of BattleShips.

**Multiplicity**: A pair of (two) _Players_ play a match together. Many pairs of _Players_ may play matches at the same time.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _Players_ setup the reef (_SetupReef_).
2. _Players_ setup their ships (_SetupShips_).
3. _Player A_ takes their turn (_TakeTurn_).
4. _Player B_ takes their turn (_TakeTurn_).
5. _System_ informs both _Players_ of the match's winner.

**Extensions**:

- (1-5)a. Any _Player_ may send a message to the other _Player_ (_SendMessage_).
- (1-2)a. If the game is in progress (eg. the game was loaded and therefore the reef and ships have already been setup); use case continues at step 3 or 4 depending on who's turn it was when the game was saved.
- (3-4)a. Either _Players_ may inform _System_ to save the match for later.
- 3a. If _Player B_ has won the game; continue use case at step 5.
- 5a. If no _Player_ has won the game; continue use case at step 3.
