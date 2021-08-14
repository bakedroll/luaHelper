#include <gtest/gtest.h>

#include "core/LuaHelperTestApplication.h"

#include <luaHelper/IModManager.h>
#include <luaHelper/ILuaStateManager.h>

#include <filesystem>
#include <fstream>

void writeStringToFile(const std::string& filename, const std::string& str)
{
  std::ofstream out(filename);
  out << str;
  out.close();
}

TEST(ModManagerTest, Loading)
{
  auto& injector = luaHelperTest::LuaHelperTestApplication::getInjector();
  auto mods      = injector.inject<luaHelper::IModManager>();
  const auto lua = injector.inject<luaHelper::ILuaStateManager>();

  std::filesystem::create_directories("./mod1/subdir");
  std::filesystem::create_directories("./mods/mod2/control");
  std::filesystem::create_directories("./mods/mod3/control");

  writeStringToFile("./mod1/control.lua", "require(\"subdir.controltest\")");
  writeStringToFile("./mod1/data.lua", "require(\"subdir.datatest\")");

  writeStringToFile("./mod1/subdir/controltest.lua", "controltest = 1");
  writeStringToFile("./mod1/subdir/datatest.lua", "datatest = 2");

  writeStringToFile("./mods/mod2/control.lua", "require(\"control.test\")");
  writeStringToFile("./mods/mod3/control.lua", "require(\"control.test\")");

  writeStringToFile("./mods/mod2/control/test.lua", "mod2test = 3");
  writeStringToFile("./mods/mod3/control/test.lua", "mod3test = 4");

  mods->loadModFromDirectory("./mod1");
  mods->scanDirectoryForMods("./mods");

  const luabridge::LuaRef controltest = lua->getGlobal("controltest");
  const luabridge::LuaRef datatest    = lua->getGlobal("datatest");
  const luabridge::LuaRef mod2test    = lua->getGlobal("mod2test");
  const luabridge::LuaRef mod3test    = lua->getGlobal("mod3test");

  ASSERT_TRUE(controltest.isNumber());
  ASSERT_TRUE(datatest.isNumber());
  ASSERT_TRUE(mod2test.isNumber());
  ASSERT_TRUE(mod3test.isNumber());

  ASSERT_EQ(static_cast<int>(controltest), 1);
  ASSERT_EQ(static_cast<int>(datatest), 2);
  ASSERT_EQ(static_cast<int>(mod2test), 3);
  ASSERT_EQ(static_cast<int>(mod3test), 4);

  std::filesystem::remove_all("./mod1");
  std::filesystem::remove_all("./mods/mod2");
  std::filesystem::remove_all("./mods/mod3");
}
