#include <gtest/gtest.h>

#include <luaHelper/ILuaStateManager.h>
#include <luaHelper/LuaTableMappedObject.h>

#include "core/LuaHelperTestApplication.h"

class MappedObject1 : public luaHelper::LuaTableMappedObject
{
public:
  MappedObject1(const luabridge::LuaRef& object, lua_State* luaState)
    : LuaTableMappedObject(object, luaState)
  {
    m_key1 = getString("key1");
    m_key2 = getNumber<int>("key2");
  }

  std::string m_key1;
  int         m_key2;
};

TEST(LuaStateManagerTest, TableHandling)
{
  auto& injector = luaHelperTest::LuaHelperTestApplication::getInjector();
  auto lua       = injector.inject<luaHelper::ILuaStateManager>();

  const luabridge::LuaRef table1 = lua->newTable();
  table1["key1"] = "string";
  table1["key2"] = 3;

  lua->setGlobal("table1", table1);

  const luabridge::LuaRef ref1 = lua->getGlobal("table1");
  const luabridge::LuaRef ref2 = lua->getGlobal("table_not_exists");
  const luabridge::LuaRef ref3 = lua->getObject("table1.key1");
  const luabridge::LuaRef ref4 = lua->getObject("table1.key2");

  EXPECT_TRUE(ref1.isTable());
  EXPECT_TRUE(ref2.isNil());
  EXPECT_TRUE(ref3.isString());
  EXPECT_TRUE(ref4.isNumber());

  EXPECT_EQ(ref3.tostring(), "string");
  EXPECT_EQ(static_cast<int>(ref4), 3);
}

TEST(LuaStateManagerTest, CodeExecution)
{
  auto& injector = luaHelperTest::LuaHelperTestApplication::getInjector();
  auto lua       = injector.inject<luaHelper::ILuaStateManager>();

  lua->executeCodeString("table2 = { key1 = \"string\", key2 = 3 }");

  const luabridge::LuaRef ref1 = lua->getObject("table1.key1");
  const luabridge::LuaRef ref2 = lua->getObject("table1.key2");

  EXPECT_TRUE(ref1.isString());
  EXPECT_TRUE(ref2.isNumber());

  EXPECT_EQ(ref1.tostring(), "string");
  EXPECT_EQ(static_cast<int>(ref2), 3);
}

TEST(LuaStateManagerTest, LuaTableMappedObject)
{
  auto& injector = luaHelperTest::LuaHelperTestApplication::getInjector();
  auto lua       = injector.inject<luaHelper::ILuaStateManager>();

  lua->executeCodeString("table3 = { key1 = \"string\", key2 = 3 }");
  const luabridge::LuaRef table3 = lua->getGlobal("table3");

  auto mo1 = lua->createTableMappedObject<MappedObject1>(table3);

  EXPECT_EQ(mo1->m_key1, "string");
  EXPECT_EQ(mo1->m_key2, 3);
}
