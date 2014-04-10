#include "JSONString.h"
#include <sstream>

JSONString::JSONString(std::string s) : JSONElement(), some_string(s)
{}

std::string JSONString::get_string() const
{
  return some_string;
}

std::string JSONString::to_string() const
{
  return "\"" + some_string + "\"";
}

JSONString JSONString::parse_string(std::string object)
{
  return JSONString(object.substr(1, object.size() - 2));
}

JSONString::~JSONString()
{

}

bool JSONString::is_json_string(JSONElement_ptr e)
{
  if ( strcmp(typeid( *e ).name(), "class JSONString") == 0 )
  {
    return true;
  }
  return false;
}

std::string JSONString::extract_string(JSONElement_ptr object)
{
  return dynamic_cast<JSONString&>( *object ).get_string();
}
