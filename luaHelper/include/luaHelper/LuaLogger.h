#pragma once

#include <luaHelper/ILuaLogger.h>
#include <osgHelper/ioc/Injector.h>

namespace luaHelper
{

class LuaLogger : public ILuaLogger
{
public:
  explicit LuaLogger(osgHelper::ioc::Injector& injector);
  ~LuaLogger() override;

  void lua_debug(std::string message) override;
  void lua_info(std::string message) override;
  void lua_warn(std::string message) override;
  void lua_fatal(std::string message) override;

};

}
