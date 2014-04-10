#include "RgbColor.h"

RgbColor::RgbColor() : RgbColor(255, 255, 255)
{}

RgbColor::RgbColor(const int r, const int g, const int b) : red(r), green(g), blue(b)
{}


RgbColor::~RgbColor()
{}

JSONObject RgbColor::serialize() const
{
  JSONObject rgb_serialize = JSONObject::get_template("Class", "RgbColor");
  rgb_serialize.set_value("red", JSONElement_ptr(new JSONString(std::to_string(red))));
  rgb_serialize.set_value("green", JSONElement_ptr(new JSONString(std::to_string(green))));
  rgb_serialize.set_value("blue", JSONElement_ptr(new JSONString(std::to_string(blue))));
  return rgb_serialize;
}

RgbColor RgbColor::unserialize(JSONObject object)
{
  int new_red = atoi(JSONString::extract_string(object["red"]).c_str());
  int new_blue = atoi(JSONString::extract_string(object["blue"]).c_str());
  int new_green = atoi(JSONString::extract_string(object["green"]).c_str());

  return RgbColor(new_red, new_green, new_blue);

}
