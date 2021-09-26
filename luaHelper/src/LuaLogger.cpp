#include <luaHelper/LuaLogger.h>

#include <utilsLib/Utils.h>

namespace luaHelper
{

LuaLogger::LuaLogger(osgHelper::ioc::Injector& injector)
  : ILuaLogger()
{
}

LuaLogger::~LuaLogger() = default;

void LuaLogger::lua_debug(std::string message)
{
  UTILS_LOG_DEBUG(message);
}

void LuaLogger::lua_info(std::string message)
{
  UTILS_LOG_INFO(message);
}

void LuaLogger::lua_warn(std::string message)
{
  UTILS_LOG_WARN(message);
}

void LuaLogger::lua_fatal(std::string message)
{
  UTILS_LOG_FATAL(message);
}

}