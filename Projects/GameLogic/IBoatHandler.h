#pragma once
#ifndef __IBOATHANDLER_H__
#define __IBOATHANDLER_H__
#include <memory>
#include "Point.h"
#include "Boat.h"

class IBoatHandler
{
public:
    virtual std::shared_ptr<Boat> get_sunken_boats() = 0;
    virtual std::shared_ptr<Boat> handle_revive_boat(Boat* salvage_tug, Boat* revived_boat) = 0;
};

#endif