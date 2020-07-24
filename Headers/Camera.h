/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-28-2020
 * File: Camera.h
 **********************************************************************************************************************/
#pragma once

#include "OpenGLIncludes.h"

class Camera
{
public:

  Camera();
  ~Camera();

  glm::vec3 GetPosition()
  {
    return Position;
  }

  glm::vec3 GetScale()
  {
    return Scale;
  }

  float GetRotation()
  {
    return Rotation;
  }

  void SetPosition(glm::vec3 position);
  void SetScale(glm::vec3 scale);
  void SetRotation(float rotation);

  glm::mat4 GetViewMatrix();

private:
  glm::mat4 ViewMatrix;
  glm::vec3 Position;
  glm::vec3 Scale;
  float Rotation;
  bool bDirty;
};
