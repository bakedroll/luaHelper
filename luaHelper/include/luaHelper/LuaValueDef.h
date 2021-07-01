#pragma once

#include <luaHelper/LuaTableMappedObject.h>

#include <memory>

namespace luaHelper
{
  class LuaValueDef : public LuaTableMappedObject
  {
  public:
    typedef std::shared_ptr<LuaValueDef> Ptr;

    explicit LuaValueDef(const luabridge::LuaRef& object, lua_State* luaState);
    ~LuaValueDef() override;

    std::string getName() const;
    std::string getGroup() const;
    float       getInit() const;
    bool        getIsVisible() const;

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

  };
}