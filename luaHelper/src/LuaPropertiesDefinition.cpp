#include <luaHelper/LuaPropertiesDefinition.h>

namespace luaHelper
{
  LuaPropertiesDefinition::LuaPropertiesDefinition(osgHelper::ioc::Injector& injector)
    : m_injector(injector)
  {
  }

  LuaPropertiesDefinition::~LuaPropertiesDefinition() = default;
}
