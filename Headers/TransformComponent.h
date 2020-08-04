/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-30-2020
 * File: TransformComponent.h
 **********************************************************************************************************************/
#pragma once

#include "OpenGLIncludes.h"

#define DEFAULT_SCALE 40.0f

class TransformComponent
{
public:
  TransformComponent();
  TransformComponent(glm::vec2 position, glm::vec2 scale, float angle = 0.0f);
  TransformComponent(const TransformComponent& rhs);
  ~TransformComponent();

  void SetPosition(glm::vec3 position);
  void SetPosition(float x, float y, float z = 0.0f);

  void SetScale(glm::vec2 scale);
  void SetScale(float x, float y);

  void SetAngle(float degrees);

  glm::mat4 GetModelMatrix();

private:
  glm::vec3 Positon;
  glm::vec2 Scale;
  float Angle;
  bool bDirty;
  glm::mat4 ModelMatrix;
};

typedef TransformComponent* TransformComponentPtr;
