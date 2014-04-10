#include "Command.h"

JSONObject Command::serialize() const
{
  JSONObject command_serialize = JSONObject::get_template("struct", "Command");
  command_serialize.set_value("id", JSONElement_ptr(new JSONString(std::to_string(id))));
  command_serialize.set_value("action", JSONElement_ptr(new JSONString(std::to_string(action))));
  command_serialize.set_value("count", JSONElement_ptr(new JSONString(std::to_string(count))));
  command_serialize.set_value("dir", JSONElement_ptr(new JSONString(std::to_string(dir))));
  command_serialize.set_value("rel_dir", JSONElement_ptr(new JSONString(std::to_string(rel_dir))));
  command_serialize.set_value("pt", JSONElement_ptr(new JSONObject(pt.serialize())));
  command_serialize.set_value("random", JSONElement_ptr(new JSONString(std::to_string(random))));

  return command_serialize;
}

Command Command::unserialize(JSONObject object)
{
  Command command;
  command.id = atol(JSONString::extract_string(object["id"]).c_str());
  command.action = static_cast<const Action::Action_enum>( atoi(JSONString::extract_string(object["action"]).c_str()) );
  command.count = atoi(JSONString::extract_string(object["count"]).c_str());
  command.dir = static_cast<Direction>( atoi(JSONString::extract_string(object["dir"]).c_str()) );;
  command.rel_dir = static_cast<RelativeDirection>( atoi(JSONString::extract_string(object["rel_dir"]).c_str()) );;
  command.pt = Point::unserialize(dynamic_cast<JSONObject&> ( *object["pt"] ));
  command.random = atoi(JSONString::extract_string(object["random"]).c_str());
  return command;
}