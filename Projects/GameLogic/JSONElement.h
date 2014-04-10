#pragma once
#include <string>
#include <memory>

class JSONElement
{
public:
  JSONElement();
  virtual std::string to_string() const;
  ~JSONElement();
};

typedef std::shared_ptr<JSONElement> JSONElement_ptr;