/*******************************************************************************************************************//**
 * \file BaseSystem.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once

#include <vector>

enum SystemType
{
  INVALID = -1,
  Graphics,
  SYSTEM_TOTAL
};

// TODO [5]: Create a base system class to make system management easier

class BaseSystem
{
public:
private:
  SystemType Type;
};
