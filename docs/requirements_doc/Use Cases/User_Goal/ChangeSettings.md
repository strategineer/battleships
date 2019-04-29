**Use Case**: ChangeSettings

**Scope**: BattleShips

**Level**: User Goal

**Intention in Context**: The intention of the _Player_ is to change their BattleShips settings.

**Multiplicity**: Many _Players_ may change their own settings at the same time.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _System_ shows _Player_ his/her current settings.
2. _Player_ requests a modification to a specific setting from _System_.
3. _System_ acknowledges _Player_ request and fulfills it.

**Extensions**:

- 2a. if _Player_ chooses to modify another setting; use case continues at step 1.
