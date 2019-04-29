**Use Case**: CreateAccount

**Scope**: BattleShips

**Level**: UserGoal

**Intention in Context**: The intention of the _Player_ is to create a BattleShips account.

**Multiplicity**: Many _Players_ can create accounts simultaneously.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _System_ shows _Player_ an empty username field, an empty password field and an empty verification field. 

        _Player_ enters their username in the username field, their password into the password field and verification field. 
2. _System_ validates username and password given by _Player_.

**Extensions**:

- 2a. If the username is already exists then _System_ informs _Player_; use case continues at step 1.
- 2b. If the password and verification fields don't match then _System_ informs _Player_; use case continues at step 1.

