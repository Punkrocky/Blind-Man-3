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
  Entity(glm::vec2 position, glm::vec3 color);
  friend void DestroyEntity(Entity* entity);

  TransformComponentPtr GetTransformComponent();
  GraphicsComponentPtr GetGraphicsComponent();


private:
  ~Entity();


  GraphicsComponentPtr GraphicsComp;
  TransformComponentPtr TransformComp;
};

typedef Entity* EntityPtr;
