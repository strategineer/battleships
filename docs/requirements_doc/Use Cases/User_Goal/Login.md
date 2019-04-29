**Use Case**: Login

**Scope**: BattleShips

**Level**: UserGoal

**Intention in Context**: The intention of the _Player_ is to login into the _System_.

**Multiplicity**: Many _Players_ can login to the _System_ at the same time.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _System_ shows _Player_ username field and password field. 
    
        _Player_ enters their username and password into their respective fields. 
2. _System_ validates username and password given by _Player_.

**Extensions**:

- 2a. _System_ realizes that it was given invalid login information:
    - 2a.1 _System_ informs _Player_ of the failed login; use case continues at step 1.
