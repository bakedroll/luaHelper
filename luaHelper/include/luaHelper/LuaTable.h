#pragma once

#include <functional>
#include <memory>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include <luaHelper/LuaEnum.h>

namespace luaHelper
{

class LuaTable
{
public:
  using IteratorFunc = std::function<void(const luabridge::Iterator&)>;

  LuaTable(const luabridge::LuaRef& object, lua_State* luaState);
  virtual ~LuaTable();

  luabridge::LuaRef& luaRef() const;

  bool                      getBoolean(const std::string& key) const;
  std::string               getString(const std::string& key) const;
  std::shared_ptr<LuaTable> getTable(const std::string& key) const;
  luabridge::LuaRef         getFunction(const std::string& key) const;

  bool                      getOptionalBoolean(const std::string& key, bool defaultValue) const;
  std::string               getOptionalString(const std::string& key, const std::string& defaultValue) const;

  template <typename Type>
  Type getUserData(const std::string& key) const
  {
    return static_cast<Type>(checkType(getRefValue(key), LUA_TUSERDATA, key));
  }

  template <typename Type>
  Type getOptionalUserData(const std::string& key, const Type& defaultValue) const
  {
    if (containsKey(key))
    {
      return getUserData<Type>(key);
    }

    return defaultValue;
  }

  template<typename T>
  T getNumber(const std::string& key) const
  {
      return static_cast<T>(checkType(getRefValue(key), LUA_TNUMBER, key));
  }

  template<typename T>
  T getOptionalNumber(const std::string& key, T defaultValue) const
  {
    if (containsKey(key))
    {
      return getNumber<T>(key);
    }

    return defaultValue;
  }

  template<typename TEnum>
  typename LuaEnum<TEnum>::Type getEnumValue(const std::string& key)
  {
    return LuaEnum<TEnum>::getMappedValue(getString(key));
  }

  template<typename TEnum>
  typename LuaEnum<TEnum>::Type getOptionalEnumValue(const std::string& key, typename LuaEnum<TEnum>::Type defaultValue)
  {
    if (containsKey(key))
    {
      return LuaEnum<TEnum>::getMappedValue(getString(key));
    }

    return defaultValue;
  }

  template<typename KeyType>
  bool containsKey(const KeyType& key) const
  {
    return !luaRef()[key].isNil();
  }

  template<typename KeyType>
  void setValue(const KeyType& key, const luabridge::LuaRef& elem)
  {
    luaRef()[key] = elem;
  }

  template<typename TEnum, typename KeyType>
  void setEnumValue(const KeyType& key, typename LuaEnum<TEnum>::Type value)
  {
    setValue(key, LuaEnum<TEnum>::getMappedString(value));
  }

  void iterateValues(const IteratorFunc& iterFunc) const;

protected:
  lua_State*        luaState() const;
  luabridge::LuaRef getRefValue(const std::string& key) const;

  static const luabridge::LuaRef& checkType(const luabridge::LuaRef& ref,
                                            int type,
                                            const std::string& key = "");

private:
  std::unique_ptr<luabridge::LuaRef> m_ref;
  lua_State*                         m_luaState;

};

}