#pragma once
#include "Serializable.h"

class RgbColor : public Serializable
{
public:
  RgbColor();
  RgbColor(const int r, const int g, const int b);
  virtual JSONObject serialize() const;
  ~RgbColor();
  int red, green, blue;
  static RgbColor unserialize(JSONObject object);
};

