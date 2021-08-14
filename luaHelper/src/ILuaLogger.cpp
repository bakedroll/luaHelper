#include <luaHelper/ILuaLogger.h>

namespace luaHelper
{

void ILuaLogger::Definition::registerDefinition(lua_State* state)
{
  getGlobalNamespace(state)
    .beginClass<ILuaLogger>("ILuaLogger")
    .addFunction("debug", &ILuaLogger::lua_debug)
    .addFunction("info", &ILuaLogger::lua_info)
    .addFunction("warn", &ILuaLogger::lua_warn)
    .addFunction("fatal", &ILuaLogger::lua_fatal)
    .endClass();
}

}