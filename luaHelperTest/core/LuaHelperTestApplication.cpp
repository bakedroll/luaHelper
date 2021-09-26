#include "core/LuaHelperTestApplication.h"

#include <luaHelper/LuaStateManager.h>
#include <luaHelper/ModManager.h>

#include <utilsLib/LoggingManager.h>

#include <gtest/gtest.h>

namespace luaHelperTest
{

int LuaHelperTestApplication::runTests(int argc, char** argv)
{
  utilsLib::ILoggingManager::create<utilsLib::LoggingManager>();

  m_staticInjector = std::make_unique<osgHelper::ioc::Injector>(m_container);
  registerComponents(m_container);

  testing::InitGoogleTest(&argc, argv);
  const auto result = RUN_ALL_TESTS();

  return result;
}

osgHelper::ioc::Injector& LuaHelperTestApplication::getInjector()
{
  return *m_staticInjector;
}

void LuaHelperTestApplication::registerComponents(osgHelper::ioc::InjectionContainer& container)
{
  container.registerSingletonInterfaceType<luaHelper::ILuaStateManager, luaHelper::LuaStateManager>();
  container.registerSingletonInterfaceType<luaHelper::IModManager, luaHelper::ModManager>();
}

std::unique_ptr<osgHelper::ioc::Injector> LuaHelperTestApplication::m_staticInjector;

}
