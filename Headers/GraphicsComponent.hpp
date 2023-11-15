/*******************************************************************************************************************//**
 * \file GraphicsComponent.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Header for Mesh, Texture, Shader, and GraphicsComponent
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "GlobalDefines.h"
#include "BaseComponent.hpp"
#include "TextureManager.hpp"
#include "OpenGLIncludes.hpp"
#include "ShaderManager.hpp"
#include "MeshManager.hpp"



/// Holds refrence data for Texture, Shader, and the colors of an attached entity
class GraphicsComponent : public BaseComponent
{
public:
  GraphicsComponent();
  GraphicsComponent(const glm::vec3& meshColor, ShaderPtr s = nullptr, TexturePtr t = nullptr, MeshPtr m = nullptr);
  GraphicsComponent(const GraphicsComponent& rhs);
  ~GraphicsComponent() = default;

  // Getters
  glm::vec4 GetColor() const;
  const Shader& GetShader() const;

  // Setters
  void SetColor(glm::vec3 color);

private:
  glm::vec4 Color;         //!< A multiplicative color to be applied on top of the attached texture
  ShaderPtr shader;        //!< A pointer to the attached Shader
  TexturePtr texture;      //!< A pointer to the attached Texture
  MeshPtr mesh;            //!< A pointer to the attached Mesh
  bool isDirty;            //!< Flag if the graphics component changed state since last update

  friend class Entity;
};

typedef GraphicsComponent* GraphicsComponentPtr;