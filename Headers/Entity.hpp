/*******************************************************************************************************************//**
 * \file Entity.h
 * \author Matthew LaDouceur
 * \date 5-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once

#include "BaseComponent.hpp"
#include "TransformComponent.hpp"
#include "GraphicsComponent.hpp"

class Entity
{
public:
  Entity();
  Entity(TransformComponentPtr transform,  GraphicsComponentPtr graphics);

  void Draw(float dt, const glm::mat4& viewMat) const;

  TransformComponentPtr GetTransformComponent() const;
  GraphicsComponentPtr GetGraphicsComponent() const;

  //void SetTransformComponent(TransformComponentPtr tranform);
  //void SetGraphicsComponent(GraphicsComponentPtr graphics);

  void AddComponent(const BaseComponent& comp);
  BaseComponent& GetComponent(ComponentType type);

  const unsigned int ID;
private:
  ~Entity();
  void InitGraphicsComponent(ShaderPtr s, TexturePtr t, MeshPtr m);

  GraphicsComponentPtr GraphicsComp;
  TransformComponentPtr TransformComp;

  friend class Engine;
};

typedef Entity* EntityPtr;
