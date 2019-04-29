**Use Case**: SetupShips

**Scope**: BattleShips

**Level**: User Goal

**Intention in Context**: The intention of the _Players_ is to setup their initial ship placement for a match of BattleShips.

**Multiplicity**: A pair of (two) _Players_ setup their ships individually at the same time for a single match of BattleShips played together. Many _Players_ may setup their ships for matches of Battleships at the same time.

**Primary Actor**: _Player_

**Main Success Scenario**:

        _System_ places ships for both _Players_ in their default positions.
1. _System_ shows _Players_ their initial ship placement configuration.
        
        _Players_ observe their ships. 
2. _Player A_ requests _System_ to move one of his/her ships to a new position.
3. _Player B_ requests _System_ to move one of his/her ships to a new position.

**Extensions**:

- 2a. If _Player A_ wishes to move another ship; continue use case at step 2.
- 3a. If _Player B_ wishes to move another ship; continue use case at step 3. 
