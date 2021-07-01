#include <luaHelper/LuaMapTable.h>

namespace luaHelper
{
  LuaMapTable::LuaMapTable(const luabridge::LuaRef& object, lua_State* luaState)
    : LuaTableMappedObject(object, luaState)
  {
  }

  LuaMapTable::~LuaMapTable() = default;
}