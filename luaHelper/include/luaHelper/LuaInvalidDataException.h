#pragma once

#include <stdexcept>
#include <string>

namespace luaHelper
{

  class LuaInvalidDataException : public std::runtime_error
  {
  public:
    explicit LuaInvalidDataException(const std::string& message);
    LuaInvalidDataException(const LuaInvalidDataException& rhs);
    ~LuaInvalidDataException() override;

  };

}
