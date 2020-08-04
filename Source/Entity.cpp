/*******************************************************************************************************************//**
 * \file Entity.cpp
 * \brief Blind Man 3
 * \author Matthew LaDouceur
 * \date 5-28-2020
 **********************************************************************************************************************/

#include "Entity.h"

Entity::Entity()
{
  // Create an empty Entity
  GraphicsComp = nullptr;
  TransformComp = nullptr;
}


Entity::Entity(const TransformComponent& transform, const GraphicsComponent& graphics)
{
  SetTransformComponent(transform);
  SetGraphicsComponent(graphics);
}


void DestroyEntity(Entity* entity)
{
  delete entity;
}


TransformComponentPtr Entity::GetTransformComponent() const
{
  return TransformComp;
}


GraphicsComponentPtr Entity::GetGraphicsComponent() const
{
  return GraphicsComp;
}



void Entity::SetTransformComponent(const TransformComponent& transform)
{
  TransformComp = new TransformComponent(transform);
}


void Entity::SetGraphicsComponent(const GraphicsComponent& graphics)
{
  GraphicsComp = new GraphicsComponent(graphics);
}

/**********************************************************************************************************************\
|*************************************************| PRIVATE MEMBERS |**************************************************|
\**********************************************************************************************************************/

Entity::~Entity()
{
  delete TransformComp;
  delete GraphicsComp;
}
