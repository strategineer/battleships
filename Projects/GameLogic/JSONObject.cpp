#include "JSONObject.h"
#include <iostream>


JSONObject::JSONObject()
{

}

void JSONObject::set_value(std::string key, JSONElement_ptr value)
{
  key_val_mapping[key] = value;
}

JSONElement_ptr JSONObject::get_element(std::string key)
{
  return key_val_mapping[key];
}

std::string JSONObject::to_string() const
{
  std::string json_string("{");
  std::map<std::string, JSONElement_ptr>::const_iterator it = key_val_mapping.begin();
  while ( it != key_val_mapping.end() )
  {
    std::string key = "\"" + ( *it ).first + "\" : ";
    json_string += key + ( *( ( *it ).second ) ).to_string();
    if ( ++it != key_val_mapping.end() )
    {
      json_string += ", ";
    }
  }
  return json_string + "}";
}

JSONObject JSONObject::parse_object(std::string object)
{
  std::map<std::string, JSONElement_ptr> kvmap;
  for ( int i = 0; object.size(); i++ )
  {
    //The key
    std::string key;
    //if first curley brace or space skip
    if ( object[i] == ' ' || object[i] == '{' )
    {
      continue;
    }
    //if quotation mark this means we are at key
    else if ( object[i] == '"' )
    {
      //Get position of first char of key
      int key_start = ++i;

      //Go to the other quotation mark
      while ( object[i] != '"' )
      {
        i++;
      }
      //Get substring from key_start and length is the current position - starting position
      key = object.substr(key_start, i - key_start);
      i++;
    }
    //Skip spaces and :
    while ( object[i] == ' ' || object[i] == ':' )
    {
      i++;
    }
    //start position of value
    int start_value = i;
    //keep the char as reminder of value type
    char value_type = object[i];
    if ( object[i] == '{' || object[i] == '[' )
    {
      //increase when opening tag, decrease when closing tag. It is zero when all tag cancel out. i.e. end of outer object or array
      //Start at 1 for the first opening brace
      int num_braces = 1;
      i++;
      while ( num_braces != 0 )
      {
        if ( object[i] == '{' || object[i] == '[' )
        {
          num_braces++;
        }
        else if ( object[i] == '}' || object[i] == ']' )
        {
          num_braces--;
        }
        i++;
      }
    }
    else if ( object[i] == '"' )
    {
      i++;
      while ( object[i] != '"' )
      {
        i++;
      }
      i++;
    }

    JSONElement_ptr value(NULL);
    //get substring corresponding to value
    std::string value_str = object.substr(start_value, i - start_value);
    //Call parse on value to get the JSONElement_ptr
    if ( value_type == '{' )
    {
      value = JSONElement_ptr(new JSONObject(JSONObject::parse_object(value_str)));
    }
    else if ( value_type == '[' )
    {
      value = JSONElement_ptr(new JSONArray(JSONArray::parse_object_array(value_str)));
    }
    else
    {
      value = JSONElement_ptr(new JSONString(JSONString::parse_string(value_str)));
    }
    //add to map
    kvmap.emplace(key, value);

    //ignore spaces
    while ( object[i] == ' ' )
    {
      i++;
    }
    //if , this means we have more key values to parse
    if ( object[i] == ',' )
    {
      continue;
    }
    //if closing curley brace this means we are done parsing
    else if ( object[i] == '}' )
    {
      break;
    }
  }
  //Create the JSONObject with map and return
  JSONObject new_object;
  for ( auto it = kvmap.begin(); it != kvmap.end(); it++ )
  {
    new_object.set_value(( *it ).first, ( *it ).second);
  }
  return new_object;
}

JSONObject::~JSONObject()
{

}

bool JSONObject::is_json_object(JSONElement_ptr e)
{
  if ( strcmp(typeid( *e ).name(), "class JSONObject") == 0 )
  {
    return true;
  }
  return false;
}

JSONObject JSONObject::get_template(std::string type, std::string class_name)
{
  JSONObject json_object;
  json_object.set_value("type", JSONElement_ptr(new JSONString(type)));
  json_object.set_value("name", JSONElement_ptr(new JSONString(class_name)));
  return json_object;
}

JSONElement_ptr JSONObject::operator[] (const std::string key)
{
  return key_val_mapping[key];
}