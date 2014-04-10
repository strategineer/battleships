#include "JSONArray.h"


void JSONArray::add(JSONElement_ptr e)
{
  elements.push_back(e);
}

std::vector<JSONElement_ptr> JSONArray::get_elements() const
{
  return std::vector<JSONElement_ptr>(elements);
}

std::string JSONArray::to_string() const
{
  std::string json_string("[");
  std::vector<JSONElement_ptr>::const_iterator it = elements.begin();
  while ( it != elements.end() )
  {
    json_string += ( **it ).to_string();

    if ( ++it != elements.end() )
    {
      json_string += ", ";
    }
  }
  return json_string + "]";
}

JSONArray JSONArray::parse_object_array(std::string array_object)
{
  std::vector<JSONElement_ptr> elements;
  int i = 0;
  while ( array_object[i] != '[' )
  {
    i++;
  }
  i++;
  for ( ; i < array_object.size(); i++ )
  {
    while ( array_object[i] == ' ' )
    {
      i++;
    }
    //start position of value
    int start_value = i;
    //keep the char as reminder of value type
    char value_type = array_object[i];
    if ( array_object[i] == '{' || array_object[i] == '[' )
    {
      //increase when opening tag, decrease when closing tag. It is zero when all tag cancel out. i.e. end of outer object or array
      //Start at 1 for the first opening brace
      int num_braces = 1;
      i++;
      while ( num_braces != 0 )
      {
        if ( array_object[i] == '{' || array_object[i] == '[' )
        {
          num_braces++;
        }
        else if ( array_object[i] == '}' || array_object[i] == ']' )
        {
          num_braces--;
        }
        i++;
      }
    }
    else if ( array_object[i] == '"' )
    {
      i++;
      while ( array_object[i] != '"' )
      {
        i++;
      }
      i++;
    }

    JSONElement_ptr value(NULL);
    //get substring corresponding to value
    std::string value_str = array_object.substr(start_value, i - start_value);
    if ( value_str == "" )
    {
      break;
    }
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
    elements.push_back(value);

    //ignore spaces
    while ( array_object[i] == ' ' )
    {
      i++;
    }
    //if , this means we have more key values to parse
    if ( array_object[i] == ',' )
    {
      continue;
    }
    //if closing curley brace this means we are done parsing
    else if ( array_object[i] == '}' )
    {
      break;
    }

  }
  JSONArray jsonArray;

  for ( auto it = elements.begin(); it != elements.end(); it++ )
  {
    jsonArray.add(( *it ));
  }

  return jsonArray;
}

bool JSONArray::is_json_array(JSONElement_ptr e)
{
  if ( strcmp(typeid( *e ).name(), "class JSONArray") == 0 )
  {
    return true;
  }
  return false;
}
