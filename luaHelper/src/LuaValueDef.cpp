#include <luaHelper/LuaValueDef.h>

namespace luaHelper
{
  struct LuaValueDef::Impl
  {
    Impl()
      : init(0.0)
      , isVisible(false)
    {}

    std::string name;
    std::string group;
    float       init;
    bool        isVisible;
  };

  LuaValueDef::LuaValueDef(const luabridge::LuaRef& object, lua_State* luaState)
    : LuaTableMappedObject(object, luaState)
    , m(new Impl())
  {
    m->name = getString("name");
    m->init = getNumber<float>("init");

    luabridge::LuaRef visible = object["visible"];
    luabridge::LuaRef group   = object["group"];

    if (containsKey("visible"))
    {
      m->isVisible = getBoolean("visible");
    }

    if (containsKey("group"))
    {
      m->group = getString("group");
    }
  }

  LuaValueDef::~LuaValueDef() = default;

  std::string LuaValueDef::getName() const
  {
    return m->name;
  }

  std::string LuaValueDef::getGroup() const
  {
    return m->group;
  }

  float LuaValueDef::getInit() const
  {
    return m->init;
  }

  bool LuaValueDef::getIsVisible() const
  {
    return m->isVisible;
  }
}
