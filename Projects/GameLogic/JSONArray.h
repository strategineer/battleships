#pragma once
#include "JSONElement.h"
#include "JSONObject.h"
#include "JSONString.h"
#include <vector>
#include <memory>

class JSONArray : public JSONElement
{
private:
  std::vector<JSONElement_ptr> elements;
public:
  void add(JSONElement_ptr e);
  std::vector<JSONElement_ptr> get_elements() const;
  virtual std::string to_string() const;
  static JSONArray parse_object_array(std::string array_object);
  static bool is_json_array(JSONElement_ptr e);
};