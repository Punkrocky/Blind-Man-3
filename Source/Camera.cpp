/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 5-28-2020
 * File: Camera.cpp
 **********************************************************************************************************************/
#include "Camera.h"

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
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    ViewMatrix = glm::ortho(-Scale.x, Scale.x, -Scale.y, Scale.y);

    ViewMatrix *= rotation;
    ViewMatrix *= translate;
    bDirty = false;
  }

  return ViewMatrix;
}
