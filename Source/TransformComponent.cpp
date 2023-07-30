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
TransformComponent::TransformComponent() : Positon(0.0f, 0.0f), Scale(DEFAULT_SCALE), isDirty(true)
{}

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
TransformComponent::TransformComponent(const glm::vec2& position, float scale, bool isdirty) : Positon(position), Scale(scale), isDirty(isdirty)
{}

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
  this->isDirty = rhs.isDirty;
}


/*!
 * \brief Set the world position of the attached entity
 *
 * \param position 
 *  The new world position to use
 *
 */
void TransformComponent::SetPosition(const glm::vec2& position)
{
  SetPosition(position.x, position.y);
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
  Positon.x = x;
  Positon.y = y;
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
 * \brief Set the world position of the attached entity
 *
 * \return glm::mat4
 *  The modle to world transform matrix
 *
 */
glm::mat4 TransformComponent::GetModelMatrix() const
{
  glm::mat4 ModelMatrix(1.0f);

  ModelMatrix[3] = glm::vec4(Positon, 1.0f, 1.0f);
  ModelMatrix[0][0] = Scale;
  ModelMatrix[1][1] = Scale;


  return ModelMatrix;
}
