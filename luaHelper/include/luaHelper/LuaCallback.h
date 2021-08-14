#pragma once

#include <luaHelper/ILuaStateManager.h>

#include <memory>
#include <vector>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

namespace luaHelper
{
  class LuaCallback
  {
  public:
    using Ptr = std::shared_ptr<LuaCallback>;

    void addFunction(const luabridge::LuaRef& func);

    template<typename... Args>
    void trigger(const ILuaStateManager::Ptr& lua, Args... args) const
    {
      lua->safeExecute([this, &args...]()
      {
        for (const auto& func : m_funcs)
        {
          func(args...);
        }
      });
    }

  private:
    using LuaFuncList = std::vector<luabridge::LuaRef>;

    LuaFuncList m_funcs;
  };

}