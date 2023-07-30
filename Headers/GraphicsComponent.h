/*******************************************************************************************************************//**
 * \file GraphicsComponent.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Header for Mesh, Texture, Shader, and GraphicsComponent
 **********************************************************************************************************************/
#pragma once
#include <vector>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "OpenGLIncludes.h"

#define VERTEX_COUNT 4
#define INDEX_COUNT 6

#define TEXTURE_PATH "../../Assets/Textures/"
#define SHADER_PATH "../../Assets/Shaders/"

/// Holds refrence data for Texture, Shader, and the colors of an attached entity
class GraphicsComponent
{
public:
  GraphicsComponent();
  GraphicsComponent(const glm::vec3& meshColor, ShaderPtr s = nullptr, TexturePtr t = nullptr, MeshPtr m = nullptr);
  GraphicsComponent(const GraphicsComponent& rhs);
  ~GraphicsComponent() = default;

  // Getters
  glm::vec4 GetColor();

  // Setters
  void SetColor(glm::vec3 color);

private:
  glm::vec4 Color;    //!< A multiplicative color to be applied on top of the attached texture
  ShaderPtr shader;   //!< A pointer to the attached Shader
  TexturePtr texture; //!< A pointer to the attached Texture
  MeshPtr mesh;       //!< A pointer to the attached Mesh

  friend class Entity;
};

typedef GraphicsComponent* GraphicsComponentPtr;