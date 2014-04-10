#pragma once
#include "JSONElement.h"
#include <string>
#include <memory>

class JSONString : public JSONElement
{
private:
  std::string some_string;
public:
  JSONString(std::string s);
  std::string get_string() const;
  virtual std::string to_string() const;
  static JSONString parse_string(std::string object);
  static bool is_json_string(JSONElement_ptr e);
  static std::string extract_string(JSONElement_ptr object);
  ~JSONString();
};