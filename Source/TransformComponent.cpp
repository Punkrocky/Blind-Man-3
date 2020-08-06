/*******************************************************************************************************************//**
 * \file TransformComponent.cpp
 * \author Matthew LaDouceur
 * \date 5-30-2020
 * \brief Source for TransformComponent
 **********************************************************************************************************************/
#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

/*!
 * \brief Default constructor
 *
 */
TransformComponent::TransformComponent()
{
  Positon = glm::vec2(0.0f);
  Scale = DEFAULT_SCALE;
  Angle = 0.0f;
  ModelMatrix = glm::mat4(1.0f);
  bDirty = true;
}

/*!
 * \brief Constructor
 *
 * \param position
 *  The world position
 *
 * \param scale
 *  The uniform scale
 *
 * \param angle
 *  The rotation in degrees, defaults to 0
 *
 */
TransformComponent::TransformComponent(glm::vec2 position, float scale, float angle)
{
  Positon = position;
  Scale = scale;
  Angle = angle;
  ModelMatrix = glm::mat4(1.0f);
  bDirty = true;
}

/*!
 * \brief Copy constructor
 *
 * \param rhs
 *  The TransformComponent being copied from
 *
 */
TransformComponent::TransformComponent(const TransformComponent& rhs)
{
  this->Positon = rhs.Positon;
  this->Scale = rhs.Scale;
  this->Angle = rhs.Angle;
  this->ModelMatrix = rhs.ModelMatrix;
  this->bDirty = rhs.bDirty;
}


/*!
 * \brief Set the world position of the attached entity
 *
 * \param position 
 *  The new world position to use
 *
 */
void TransformComponent::SetPosition(glm::vec2 position)
{
  Positon = position;
}


/*!
 * \brief Set the world position of the attached entity using 3 floats
 *
 * \param x
 *  The new x value
 *
 * \param y
 *  The new y value
 *
 * \param z
 *  The new z value
 *
 */
void TransformComponent::SetPosition(float x, float y)
{
  SetPosition(glm::vec2(x, y));
}

/*!
 * \brief Set the world scale of the attached entity
 *
 * \param scale
 *  The new world scale to use
 *
 */
void TransformComponent::SetScale(float scale)
{
  Scale = scale;
}

/*!
 * \brief Set the world rotation of the attached entity
 *
 * \param degrees
 *  The new world rotation to use
 *
 */
void TransformComponent::SetAngle(float degrees)
{
  Angle = degrees;
}

/*!
 * \brief Set the world position of the attached entity
 *
 * \return glm::mat4
 *  The modle to world transform matrix
 *
 */
glm::mat4 TransformComponent::GetModelMatrix()
{
  // Recalculate the modle to world matrix if bDirty is true
  if (bDirty)
  {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(Positon, 0.0f));
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(Scale, Scale, 1.0f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), Angle, glm::vec3(0.0f, 0.0f, 1.0f));

    ModelMatrix = translate * rotate * scale;
  }

  return ModelMatrix;
}
