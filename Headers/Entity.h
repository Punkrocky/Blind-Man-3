/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-28-2020
 * File: Entity.h
 **********************************************************************************************************************/
#pragma once

#include "GraphicsComponent.h"
#include "TransformComponent.h"

#include <typeinfo>

class Entity
{
public:
  Entity();
  friend void DestroyEntity(Entity* entity);

  TransformComponent* GetTransformComponent()
  {
    return TransformComp;
  }

  GraphicsComponent* GetGraphicsComponent()
  {
    return GraphicsComp;
  }

private:
  ~Entity();


  GraphicsComponent* GraphicsComp;
  TransformComponent* TransformComp;
};
