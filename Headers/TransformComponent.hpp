/*******************************************************************************************************************//**
 * \file TransformComponent.h
 * \author Matthew LaDouceur
 * \date 5-30-2020
 * \brief Header for TransformComponent
 **********************************************************************************************************************/
#pragma once

#include "GraphicsComponent.hpp"
#include "OpenGLIncludes.hpp"
#include "GlobalDefines.h"

/// Holds data related to how an entity should be orientated in the world
class TransformComponent
{
public:
  TransformComponent();
  TransformComponent(const glm::vec2& position, float scale = TILE_HALF_SIZE, bool isdirty = true);
  TransformComponent(const TransformComponent& rhs);
  ~TransformComponent() = default;

  void SetPosition(const glm::vec2& position);
  void SetPosition(float x, float y);
  void SetScale(float scale);

  glm::mat4 GetModelMatrix() const;
private:
  glm::vec2 Positon; //!< World position of the mesh
  float Scale;       //!< Uniform value to scale our mesh by in world value
  bool isDirty;      //!< Determines if the matrix needs to be recalculated
};

typedef TransformComponent* TransformComponentPtr;
