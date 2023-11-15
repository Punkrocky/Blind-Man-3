/*******************************************************************************************************************//**
 * \file GraphicsComponent.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Source for Mesh, Texture, Shader, and GraphicsComponent
 **********************************************************************************************************************/
#include "GraphicsComponent.hpp"


GraphicsComponent::GraphicsComponent() : Color(1.0f), shader(nullptr), texture(nullptr), mesh(nullptr), isDirty(true)
{}


// Convertion constructor for glm::vec3
GraphicsComponent::GraphicsComponent(const glm::vec3& meshColor, ShaderPtr s, TexturePtr t, MeshPtr m) : Color(glm::vec4(meshColor, 1.0f)), shader(s), texture(t), mesh(m)
{}


GraphicsComponent::GraphicsComponent(const GraphicsComponent& rhs) : shader(rhs.shader), texture(rhs.texture), mesh(rhs.mesh)
{
  this->AttachedID = rhs.AttachedID;
  this->Color = rhs.Color;
}


glm::vec4 GraphicsComponent::GetColor() const
{
  return Color;
}

const Shader& GraphicsComponent::GetShader() const
{
  return *shader;
}


void GraphicsComponent::SetColor(glm::vec3 color)
{
  if (color != glm::vec3(Color))
  {
    Color = glm::vec4(color, 1.0f);
    mesh->SetColor(Color);
  }
}
