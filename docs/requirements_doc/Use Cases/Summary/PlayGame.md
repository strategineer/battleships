**Use Case**: PlayGame

**Scope**: BattleShips

**Level**: Summary

**Intention in Context**: The intention of the _Player_ is to play BattleShips.

**Multiplicity**: Many _Players_ can play the game at the same time. _Players_ do not interact with the _System_ directly; instead each _Player_ uses a _Mouse_ and _Keyboard_ to perform their interactions. The _System_ does not communicate through telepathy with the _Player_ instead it chooses to display information graphically through the _Display_. 

**Primary Actor**: _Player_

**Facilitator Actors**: _Mouse_ and _Keyboard_

**Secondary Actor**: _Display_

**Main Success Scenario**:

1. _System_ shows _Player_ the login screen.
2. _Player_ can choose to: log in (_Login_) or create an account (_CreateAccount_).

		_Player_ can repeat step 2 as many times as desired.
3_System_ presents _Player_ with the main menu.
4. _Player_ may choose: to hangout with another player (_Hangout_), to spectate a game (_SpectateGame_) or to change their settings (_ChangeSettings_).
5. _Player_ requests _System_ to quit BattleShips.

**Extensions**:
- 4a. If _Player_ does not have valid login credentials; continue use case at step 2.
