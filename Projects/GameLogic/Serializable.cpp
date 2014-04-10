#include "Serializable.h"

Serializable::Serializable()
{

}

Serializable::~Serializable()
{

}

JSONObject Serializable::serialize() const
{
  return JSONObject();
}