/*******************************************************************************************************************//**
 * \file TransformComponent.h
 * \author Matthew LaDouceur
 * \date 5-30-2020
 * \brief Header for TransformComponent
 **********************************************************************************************************************/
#pragma once

#include "OpenGLIncludes.h"

#define DEFAULT_SCALE 40.0f

/// Holds data related to how an entity should be orientated in the world
class TransformComponent
{
public:
  TransformComponent();
  TransformComponent(glm::vec2 position, float scale, float angle = 0.0f);
  TransformComponent(const TransformComponent& rhs);
  ~TransformComponent() = default;

  void SetPosition(glm::vec2 position);
  void SetPosition(float x, float y);
  void SetScale(float scale);
  void SetAngle(float degrees);

  glm::mat4 GetModelMatrix();
private:
  bool bDirty;           //!< True if we need to recalculate the ModleMatrix
  float Scale;           //!< Uniform value to scale our mesh by in world value
  float Angle;           //!< Degrees the mesh is rotated by
  glm::vec2 Positon;     //!< World position of the mesh
  glm::mat4 ModelMatrix; //!< Composition of the above data into a modle to world transform matrix
};

typedef TransformComponent* TransformComponentPtr;
