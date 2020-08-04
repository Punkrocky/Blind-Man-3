/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-28-2020
 * File: Entity.h
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
