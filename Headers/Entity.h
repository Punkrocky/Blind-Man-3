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
  Entity(TransformComponentPtr transform,  GraphicsComponentPtr graphics);

  void Draw(const glm::mat4& viewMat) const;

  TransformComponentPtr GetTransformComponent() const;
  GraphicsComponentPtr GetGraphicsComponent() const;

  //void SetTransformComponent(TransformComponentPtr tranform);
  //void SetGraphicsComponent(GraphicsComponentPtr graphics);

private:
  ~Entity();
  void InitGraphicsComponent(ShaderPtr s, TexturePtr t, MeshPtr m);

  GraphicsComponentPtr GraphicsComp;
  TransformComponentPtr TransformComp;

  friend class Engine;
};

typedef Entity* EntityPtr;
