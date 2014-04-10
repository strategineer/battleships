#pragma once
#include <string>
#include <map>
#include "JSONElement.h"
#include "JSONArray.h"
#include "JSONString.h"
#include <memory>

class JSONObject : public JSONElement
{
private:
  std::map<std::string, JSONElement_ptr> key_val_mapping;
public:
  JSONObject();
  void set_value(std::string key, JSONElement_ptr value);
  JSONElement_ptr get_element(std::string key);
  virtual std::string to_string() const;
  static JSONObject parse_object(std::string object);
  static bool is_json_object(JSONElement_ptr e);
  static JSONObject get_template(std::string type, std::string class_name);
  JSONElement_ptr operator[] (const std::string key);
  ~JSONObject();
};

