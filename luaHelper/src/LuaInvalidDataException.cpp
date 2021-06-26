#include <luaHelper/LuaInvalidDataException.h>

namespace luaHelper
{

LuaInvalidDataException::LuaInvalidDataException(const std::string& message)
  : std::runtime_error(std::string("Invalid lua data exception: ") + message)
{
}

LuaInvalidDataException::LuaInvalidDataException(const LuaInvalidDataException& rhs)
  : std::runtime_error(rhs.what())
{
}

LuaInvalidDataException::~LuaInvalidDataException() = default;

}
