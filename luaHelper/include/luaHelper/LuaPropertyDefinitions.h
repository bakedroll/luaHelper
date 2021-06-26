#pragma once

#include <luaHelper/LuaBridgeDefinition.h>
#include <luaHelper/LuaStaticProperty.h>

#include <osgHelper/ioc/Injector.h>

namespace luaHelper
{
    class LuaPropertyDefinitions : public LuaBridgeDefinition
    {
    public:
      explicit LuaPropertyDefinitions(osgHelper::ioc::Injector& injector);
      virtual ~LuaPropertyDefinitions();

    private:
      osgHelper::ioc::Injector& m_injector;

      template <typename T>
      void addProperty(lua_State* state, const char* name)
      {
        LuaStaticProperty<T>::set(m_injector.inject<T>().get());
        getGlobalNamespace(state)
          .beginNamespace("lua")
          .addProperty(name, LuaStaticProperty<T>::get)
          .endNamespace();
      }

    };
}
