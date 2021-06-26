#include <luaHelper/LuaMapTable.h>

namespace luaHelper
{
  LuaMapTable::LuaMapTable(const luabridge::LuaRef& object, lua_State* luaState)
    : LuaObjectMapper(object, luaState)
  {
  }

  LuaMapTable::~LuaMapTable() = default;
}