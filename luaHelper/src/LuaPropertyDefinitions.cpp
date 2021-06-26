#include <luaHelper/LuaPropertyDefinitions.h>

namespace luaHelper
{
  LuaPropertyDefinitions::LuaPropertyDefinitions(osgHelper::ioc::Injector& injector)
    : m_injector(injector)
  {
  }

  LuaPropertyDefinitions::~LuaPropertyDefinitions() = default;
}
