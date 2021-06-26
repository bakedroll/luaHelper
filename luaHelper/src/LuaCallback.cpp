#include <luaHelper/LuaCallback.h>

namespace luaHelper
{
  void LuaCallback::addFunction(const luabridge::LuaRef& func)
  {
    m_funcs.push_back(func);
  }
}