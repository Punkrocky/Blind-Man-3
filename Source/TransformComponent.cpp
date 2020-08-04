/*******************************************************************************************************************//**
 * \file TransformComponent.cpp
 * \author Matthew LaDouceur
 * \date 5-30-2020
 * \brief
 **********************************************************************************************************************/
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

TransformComponent::TransformComponent()
{
  Positon = glm::vec3(0.0f, 0.0f, 0.0f);
  Scale = glm::vec2(DEFAULT_SCALE, DEFAULT_SCALE);
  Angle = 0.0f;
  ModelMatrix = glm::mat4(1.0f);
  bDirty = true;
}

TransformComponent::TransformComponent(glm::vec2 position, glm::vec2 scale, float angle)
{
  Positon = glm::vec3(position, 0.0f);
  Scale = scale;
  Angle = angle;
  ModelMatrix = glm::mat4(1.0f);
  bDirty = true;
}

TransformComponent::TransformComponent(const TransformComponent& rhs)
{
  this->Positon = rhs.Positon;
  this->Scale = rhs.Scale;
  this->Angle = rhs.Angle;
  this->ModelMatrix = rhs.ModelMatrix;
  this->bDirty = rhs.bDirty;
}

TransformComponent::~TransformComponent(){}


void TransformComponent::SetPosition(glm::vec3 position)
{
  Positon = position;
}

void TransformComponent::SetPosition(float x, float y, float z)
{
  SetPosition(glm::vec3(x, y, z));
}


void TransformComponent::SetScale(glm::vec2 scale)
{
  Scale = scale;
}

void TransformComponent::SetScale(float x, float y)
{
  SetScale(glm::vec2(x, y));
}


void TransformComponent::SetAngle(float degrees)
{
  Angle = degrees;
}


glm::mat4 TransformComponent::GetModelMatrix()
{
  if (bDirty)
  {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), Positon);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(Scale.x, Scale.y, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    ModelMatrix = translate * rotate * scale;
  }

  return ModelMatrix;
}
