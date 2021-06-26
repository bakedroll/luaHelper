#include <luaHelper/LuaBridgeDefinition.h>

namespace luaHelper
{
  luans::Namespace LuaBridgeDefinition::getGlobalNamespace(lua_State* L)
  {
    return luans::getGlobalNamespace(L);
  }
}
