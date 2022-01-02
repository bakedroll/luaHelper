#pragma once

#include <map>
#include <string>
#include <typeindex>

#include <luaHelper/LuaInvalidDataException.h>

namespace luaHelper
{

template <typename T>
class LuaEnum
{
public:
  using Type = T;

  static void registerValue(T value, const std::string& mappedString)
  {
    m_valueStringMap[value] = mappedString;
  }

  static std::string getMappedString(T value)
  {
    if (m_valueStringMap.count(value) == 0)
    {
      const std::string typeName(typeid(T).name());
      throw LuaInvalidDataException("Unknown enum value for enum type " + typeName + ".");
    }
    return m_valueStringMap.at(value);
  }

  static T getMappedValue(const std::string& mappedString)
  {
    for (const auto& str : m_valueStringMap)
    {
      if (str.second == mappedString)
      {
        return str.first;
      }
    }

    const std::string typeName(typeid(T).name());
    throw LuaInvalidDataException("Invalid enum value for enum type " + typeName + ": '" + mappedString + "'.");
  }

private:
  static std::map<T, std::string> m_valueStringMap;

};

template <typename T>
std::map<T, std::string> LuaEnum<T>::m_valueStringMap;

}
