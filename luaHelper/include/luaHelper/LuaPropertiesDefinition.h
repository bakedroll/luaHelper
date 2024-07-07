#pragma once

#include <luaHelper/LuaBridgeDefinition.h>
#include <luaHelper/LuaStaticProperty.h>

#include <osgHelper/ioc/Injector.h>

namespace luaHelper
{
  class LuaPropertiesDefinition : public LuaBridgeDefinition
  {
  public:
    explicit LuaPropertiesDefinition(osgHelper::ioc::Injector& injector);
    ~LuaPropertiesDefinition() override;

  protected:

    template <typename T>
    void addProperty(lua_State* state, const char* name, const char* ns = "lua")
    {
      LuaStaticProperty<T>::set(m_injector.inject<T>().get());
      getGlobalNamespace(state)
        .beginNamespace(ns)
        .addProperty(name, LuaStaticProperty<T>::get)
        .endNamespace();
    }

  private:
    osgHelper::ioc::Injector& m_injector;

  };
}
