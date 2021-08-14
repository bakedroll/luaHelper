#pragma once

#include <osg/Referenced>
#include <osg/ref_ptr>

#include <luaHelper/LuaBridgeDefinition.h>

namespace luaHelper
{

class ILuaLogger : public osg::Referenced
{
public:
  using Ptr = osg::ref_ptr<ILuaLogger>;

  class Definition : public LuaBridgeDefinition
  {
  public:
    void registerDefinition(lua_State* state) override;
  };

  ILuaLogger() = default;
  virtual ~ILuaLogger() = default;

  virtual void lua_debug(std::string message) = 0;
  virtual void lua_info(std::string message) = 0;
  virtual void lua_warn(std::string message) = 0;
  virtual void lua_fatal(std::string message) = 0;

};

}