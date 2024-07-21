#include <luaHelper/LuaGenericException.h>

namespace luaHelper
{

LuaGenericException::LuaGenericException(const std::string& message)
  : std::runtime_error(std::string("Generic lua exception: ") + message)
{
}

LuaGenericException::LuaGenericException(const LuaGenericException& rhs)
  : std::runtime_error(rhs.what())
{
}

LuaGenericException::~LuaGenericException() = default;

}
