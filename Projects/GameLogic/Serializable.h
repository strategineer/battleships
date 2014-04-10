#pragma once
#include "JSON.h"

class Serializable
{
public:
  Serializable();
  virtual JSONObject serialize() const;
  ~Serializable();

};