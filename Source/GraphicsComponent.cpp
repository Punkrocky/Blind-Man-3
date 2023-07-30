/*******************************************************************************************************************//**
 * \file GraphicsComponent.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Source for Mesh, Texture, Shader, and GraphicsComponent
 **********************************************************************************************************************/
#include "GraphicsComponent.h"


GraphicsComponent::GraphicsComponent() : Color(1.0f), shader(nullptr), texture(nullptr), mesh(nullptr)
{}


GraphicsComponent::GraphicsComponent(const glm::vec3& meshColor, ShaderPtr s, TexturePtr t, MeshPtr m) : Color(glm::vec4(meshColor, 1.0f)), shader(s), texture(t), mesh(m)
{}


GraphicsComponent::GraphicsComponent(const GraphicsComponent& rhs) : shader(rhs.shader), texture(rhs.texture), mesh(rhs.mesh)
{
  this->Color = rhs.Color;
}


glm::vec4 GraphicsComponent::GetColor()
{
  return Color;
}


void GraphicsComponent::SetColor(glm::vec3 color)
{
  Color = glm::vec4(color, 1.0f);
}
