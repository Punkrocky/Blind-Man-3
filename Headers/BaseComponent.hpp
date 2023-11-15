 /******************************************************************************************************************//**
  * \file Serializer.h
  * \author Matthew LaDouceur
  * \date 7-28-2020
  * \brief Blind Man 3
  *********************************************************************************************************************/
#pragma once

enum ComponentType
{
  INVALID_COMPONENT = -1,
  Transform_c,
  Graphics_c,
  COMPONENT_TOTAL
};

// TODO [3]: Finish the base component class to make serialization easier

class BaseComponent
{
public:


protected:
  unsigned int AttachedID; //!< ID of the entity this component is attached to
  ComponentType Type;
private:
};
