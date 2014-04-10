#pragma once
#ifndef __ICOLLISIONDETECTOR_H__
#define __ICOLLISIONDETECTOR_H__
#include <memory>
#include "ICollidable.h"
#include "Point.h"

class ICollisionDetector
{
public:
  virtual std::shared_ptr<ICollidable> collision_check(const Point& pt) = 0;
};

#endif