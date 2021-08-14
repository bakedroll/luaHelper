#pragma once

#include <luaHelper/ILuaStateManager.h>

#include <osg/Referenced>
#include <osg/ref_ptr>

#include <string>

namespace luaHelper
{

class IModManager : public osg::Referenced
{
public:
  using Ptr = osg::ref_ptr<IModManager>;

  IModManager() = default;
  virtual ~IModManager() = default;

  virtual void loadModFromDirectory(const std::string& path) = 0;
  virtual void scanDirectoryForMods(const std::string& path) = 0;

};

}