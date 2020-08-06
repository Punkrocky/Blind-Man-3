/*******************************************************************************************************************//**
 * \file BaseSystem.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "Entity.h"
// TODO [5]: Create a base system class to make system management easier

/// The base class for all systems to inharent from
class BaseSystem
{
public:
  /// All the unique systems in the engine
  enum class SystemType
  {
    INVALID_SYSTEM_TYPE = -1,
    Graphics_s,
    SYSTEM_TYPE_TOTAL
  };

  BaseSystem(SystemType type = SystemType::INVALID_SYSTEM_TYPE) : Type(type) {}
  BaseSystem(const BaseSystem& rhs);
  ~BaseSystem();

  //virtual void Init();
  virtual void Update(float dt);
  //virtual void Shutdown();

  virtual void CreateComponentList(const std::vector<EntityPtr>& entities);

  SystemType GetSystemType();
private:
  SystemType Type; //!< 
};
