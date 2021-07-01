#pragma once

#include <osgHelper/ioc/Injector.h>

#include <luaHelper/LuaStateManager.h>

namespace luaHelper
{
  class LuaLogger : public osg::Referenced
  {
  public:
    using Ptr = osg::ref_ptr<LuaLogger>;

    class Definition : public LuaBridgeDefinition
    {
    public:
      void registerDefinition(lua_State* state) override;
    };

    explicit LuaLogger(osgHelper::ioc::Injector& injector);
    ~LuaLogger();

    void lua_debug(std::string message);
    void lua_info(std::string message);
    void lua_warn(std::string message);
    void lua_fatal(std::string message);

  };
}