#pragma once

#include <stdexcept>
#include <string>

namespace luaHelper
{

class LuaGenericException : public std::runtime_error
{
public:
  explicit LuaGenericException(const std::string& message);
  LuaGenericException(const LuaGenericException& rhs);
  ~LuaGenericException() override;

};

}
