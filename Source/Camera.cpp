/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-28-2020
 * File: Camera.cpp
 **********************************************************************************************************************/
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
  Position = glm::vec3(0, 0, -1);
  ViewMatrix = glm::mat4(1.0f);
  bDirty = true;
}

Camera::~Camera()
{

}

void Camera::SetPosition(glm::vec3 position)
{
  Position = position;
  bDirty = true;
}

void Camera::SetScale(glm::vec3 scale)
{
  Scale = scale;
  bDirty = true;
}

void Camera::SetRotation(float rotation)
{
  Rotation = rotation;
  bDirty = true;
}

glm::mat4 Camera::GetViewMatrix()
{
  if (bDirty)
  {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), Position);
    ViewMatrix = glm::ortho(-Scale.x/2, Scale.x/2, -Scale.y/2, Scale.y/2);

    ViewMatrix *= translate;
    bDirty = false;
  }

  return ViewMatrix;
}
