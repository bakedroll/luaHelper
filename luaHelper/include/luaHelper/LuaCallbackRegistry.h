#pragma once

#include <luaHelper/LuaCallback.h>
#include <luaHelper/LuaBridgeDefinition.h>
#include <luaHelper/ILuaStateManager.h>

#include <utilsLib/Utils.h>

#include <osg/Referenced>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

namespace luaHelper
{
  class LuaCallbackRegistry : public osg::Referenced
  {
  public:
    class Definition : public LuaBridgeDefinition
    {
    public:
      void registerDefinition(lua_State* state) override;
    };

    explicit LuaCallbackRegistry(const ILuaStateManager::Ptr& lua);

    void luaOnEvent(int eventId, luabridge::LuaRef func);

    template <typename... Args>
    void triggerLuaCallback(int id, Args... args)
    {
      const auto& it = m_callbacks.find(id);
      assert_return(it != m_callbacks.end());

      it->second->trigger(m_lua, args...);
    }

  protected:
    void registerLuaCallback(int id, LuaCallback::Ptr callback = nullptr);

  private:
    using CallbackMap = std::map<int, LuaCallback::Ptr>;

    CallbackMap           m_callbacks;
    ILuaStateManager::Ptr m_lua;
  };
}