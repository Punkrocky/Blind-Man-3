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

void DestroyEntity(Entity* entity)
{
  delete entity;
}

Entity::~Entity()
{
  delete TransformComp;
  delete GraphicsComp;
}
