#pragma once
#ifndef __IMINEHANDLER_H__
#define __IMINEHANDLER_H__
#include "Mine.h"

class IMineHandler
{
public:
  virtual bool add_mine(const Mine& mine) = 0;
  virtual bool remove_mine(const Point& mine_at) = 0;
};


#endif