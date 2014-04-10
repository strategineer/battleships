#pragma once
#include "Event.h"
#include "Game.h"
#include "Command.h"

class CommandEvent : public Event
{
public:
    CommandEvent(Command pcommand, Player pplayer);
    Player get_player() const;
    Command get_command() const;
private:
    Player player;
    Command command;
};