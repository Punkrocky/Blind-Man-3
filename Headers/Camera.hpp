/*******************************************************************************************************************//**
 * \file Camera.h
 * \author Matthew LaDouceur
 * \date 5-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once

#include <ostream>

#include "OpenGLIncludes.hpp"

class Camera
{
public:
  Camera();
  ~Camera();

  friend std::ostream& operator<<(std::ostream& os, const Camera& obj);

  inline glm::vec3 GetPosition() const { return Position; }
  inline glm::vec3 GetScale() const { return Scale; }
  inline float GetRotation() const { return Rotation; }

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
