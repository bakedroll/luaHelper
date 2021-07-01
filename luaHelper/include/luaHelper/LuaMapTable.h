#pragma once

#include <luaHelper/LuaTableMappedObject.h>

namespace luaHelper
{
  class LuaMapTable : public LuaTableMappedObject
  {
  public:
    using Ptr = std::shared_ptr<LuaMapTable>;

    LuaMapTable(const luabridge::LuaRef& object, lua_State* luaState);
    ~LuaMapTable();
    
    template<typename KeyType>
    bool contains(const KeyType& key) const
    {
      return !luaRef()[key].isNil();
    }

    template<typename KeyType>
    void insert(const KeyType& key, const luabridge::LuaRef& elem)
    {
      luaRef()[key] = elem;
    }

    template<typename KeyType>
    luabridge::LuaRef getElement(const KeyType& key) const
    {
      return luaRef()[key];
    }
  
  };
}