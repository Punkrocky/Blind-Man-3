/*******************************************************************************************************************//**
 * \file Entity.h
 * \author Matthew LaDouceur
 * \date 5-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once

#include "GraphicsComponent.h"
#include "TransformComponent.h"

class Entity
{
public:
  Entity();
  Entity(const TransformComponent& transform, const GraphicsComponent& graphics);
  friend void DestroyEntity(Entity* entity);

  TransformComponentPtr GetTransformComponent() const;
  GraphicsComponentPtr GetGraphicsComponent() const;

  void SetTransformComponent(const TransformComponent& tranform);
  void SetGraphicsComponent(const GraphicsComponent& graphics);

private:
  ~Entity();


  GraphicsComponentPtr GraphicsComp;
  TransformComponentPtr TransformComp;
};

typedef Entity* EntityPtr;
