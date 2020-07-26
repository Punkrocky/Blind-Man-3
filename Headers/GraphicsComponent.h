/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsComponent.h
 **********************************************************************************************************************/
#pragma once
#include <vector>

#include "OpenGLIncludes.h"

#define VERTEX_COUNT 4

class Mesh
{
public:
  Mesh();
  ~Mesh();

  size_t GetIndexCount();

  std::vector<glm::vec3>& GetVertexPositions();
  std::vector<glm::vec4>& GetVertexColors();
  std::vector<glm::vec2>& GetVertexTextureCoords();
  std::vector<unsigned int>& GetIndices();

  void SetVertexColors(glm::vec3 color)
  {
    for (int i = 0; i < VERTEX_COUNT; ++i)
    {
      VertexColors[i] = glm::vec4(color, 1.0f);
    }
  }

private:
  std::vector<glm::vec3> VertexPositions; // XYZ
  std::vector<glm::vec4> VertexColors;    // RGBA
  std::vector<glm::vec2> VertexTextureCoords;  // UV
  std::vector<unsigned int> Indices;
  //int VertexCount;
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
  GraphicsComponent()
  {
    mesh = new Mesh;
  }

  GraphicsComponent(glm::vec3 color)
  {
    mesh = new Mesh;
    mesh->SetVertexColors(color);
  }

  ~GraphicsComponent()
  {
    delete mesh;
  }

  Mesh* GetMesh()
  {
    return mesh;
  }

private:
  Mesh* mesh;
  //Texture* texture;
  //Shader* shader;
};

typedef GraphicsComponent* GraphicsComponentPtr;