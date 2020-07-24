/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-28-2020
 * File: Entity.cpp
 **********************************************************************************************************************/

#include "Entity.h"

Entity::Entity()
{
  GraphicsComp = new GraphicsComponent;
  TransformComp = new TransformComponent;
}

Entity::Entity(glm::vec2 position, glm::vec3 color)
{
  GraphicsComp = new GraphicsComponent(color);
  TransformComp = new TransformComponent(position, glm::vec2(DEFAULT_SCALE, DEFAULT_SCALE));
}

void DestroyEntity(Entity* entity)
{
  delete entity;
}

TransformComponentPtr Entity::GetTransformComponent()
{
  return TransformComp;
}

GraphicsComponentPtr Entity::GetGraphicsComponent()
{
  return GraphicsComp;
}

Entity::~Entity()
{
  delete TransformComp;
  delete GraphicsComp;
}
