**Use Case**: LoadMatch

**Scope**: BattleShips

**Level**: User Goal

**Intention in Context**: The intention of one of the two _Players_ hanging out in a room is to load a previously saved match of BattleShips.

**Multiplicity**: One _Player_ in a room may load a match at a time. Many _Players_ in seperate rooms may load matches simultaneously.

**Primary Actor**: _Player_

**Main Success Scenario**:

1. _System_ shows _Player A_ list of previously saved matches. 

        _Player A_ observes the list of matches.
2. _Player A_ tells _System_ of their choice of match to load.

        _System_ loads desired match for play.

**Extensions**:

- 2a. If _Player A_ does not wish to load any of the shown matches; use case fails.
