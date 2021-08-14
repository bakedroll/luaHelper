#include <luaHelper/LuaLogger.h>

#include <osgHelper/LogManager.h>

namespace luaHelper
{

LuaLogger::LuaLogger(osgHelper::ioc::Injector& injector)
  : ILuaLogger()
{
}

LuaLogger::~LuaLogger() = default;

void LuaLogger::lua_debug(std::string message)
{
  OSGH_LOG_DEBUG(message);
}

void LuaLogger::lua_info(std::string message)
{
  OSGH_LOG_INFO(message);
}

void LuaLogger::lua_warn(std::string message)
{
  OSGH_LOG_WARN(message);
}

void LuaLogger::lua_fatal(std::string message)
{
  OSGH_LOG_FATAL(message);
}

}