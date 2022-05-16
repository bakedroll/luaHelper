#include <luaHelper/LuaTable.h>
#include <luaHelper/LuaInvalidDataException.h>

namespace luaHelper
{

LuaTable::LuaTable(const luabridge::LuaRef& object, lua_State* luaState)
  : m_ref(std::make_unique<luabridge::LuaRef>(object))
  , m_luaState(luaState)
{
}

LuaTable::~LuaTable() = default;

luabridge::LuaRef& LuaTable::luaRef() const
{
  assert(m_ref);
  return *m_ref;
}

bool LuaTable::getBoolean(const std::string& key) const
{
  return checkType(getRefValue(key), LUA_TBOOLEAN, key);
}

std::string LuaTable::getString(const std::string& key) const
{
  return checkType(getRefValue(key), LUA_TSTRING, key).tostring();
}

std::shared_ptr<LuaTable> LuaTable::getTable(const std::string& key) const
{
  return std::make_shared<LuaTable>(checkType(getRefValue(key), LUA_TTABLE, key), m_luaState);
}

luabridge::LuaRef LuaTable::getFunction(const std::string& key) const
{
  return checkType(getRefValue(key), LUA_TFUNCTION, key);
}

bool LuaTable::getOptionalBoolean(const std::string& key, bool defaultValue) const
{
  if (containsKey(key))
  {
    return getBoolean(key);
  }

  return defaultValue;
}

std::string LuaTable::getOptionalString(const std::string& key, const std::string& defaultValue) const
{
  if (containsKey(key))
  {
    return getString(key);
  }

  return defaultValue;
}

std::shared_ptr<LuaTable> LuaTable::getOptionalTable(const std::string& key) const
{
  if (containsKey(key))
  {
    return getTable(key);
  }

  return nullptr;
}

void LuaTable::iterateValues(const IteratorFunc& iterFunc) const
{
  for (luabridge::Iterator it(*m_ref); !it.isNil(); ++it)
  {
    iterFunc(it);
  }
}

void LuaTable::iterateTables(const TablesIteratorFunc& iterFunc) const
{
  for (luabridge::Iterator it(*m_ref); !it.isNil(); ++it)
  {
    const auto table = std::make_shared<LuaTable>(checkType(it.value(), LUA_TTABLE), m_luaState);
    iterFunc(table);
  }
}

lua_State* LuaTable::luaState() const
{
  return m_luaState;
}

luabridge::LuaRef LuaTable::getRefValue(const std::string& key) const
{
  luabridge::LuaRef value = luaRef()[key];
  if (value.isNil())
  {
    throw LuaInvalidDataException(std::string("Expected key '") + key + "'");
  }

  return value;
}

const luabridge::LuaRef& LuaTable::checkType(const luabridge::LuaRef& ref, int type, const std::string& key)
{
  if (ref.type() != type)
  {
    throw LuaInvalidDataException(key.empty() ? "Value in array has wrong type."
                                              : std::string("Value '") + key + "' has wrong type.");
  }

  return ref;
}

}
