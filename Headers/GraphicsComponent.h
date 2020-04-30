/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsComponent.h
 **********************************************************************************************************************/
#pragma once
#include <vector>

#include "OpenGLIncludes.h"

class Mesh
{
public:
  Mesh();
  ~Mesh();

  std::vector<glm::vec3> VertexPosition; // XYZW
  std::vector<glm::vec4> VertexColor;    // RGBA
  std::vector<glm::vec2> VertexTexture;  // UV
  std::vector<unsigned int> Indices;
};

class Texture
{
public:
private:
};

class Shader
{
public:
  Shader(GLuint ID)
  {
    ShaderLocation = ID;
  }

  GLuint GetShaderLocation()
  {
    return ShaderLocation;
  }
private:
  GLuint ShaderLocation;
};

class GraphicsComponent
{
public:
private:
};
