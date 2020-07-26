/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsComponent.cpp
 **********************************************************************************************************************/
#include "GraphicsComponent.h"

Mesh::Mesh()
{
  // Position Data
  VertexPositions.resize(VERTEX_COUNT);
  VertexPositions[0] = glm::vec3( 1.0f,  1.0f, 0.0f); // Top Right
  VertexPositions[1] = glm::vec3( 1.0f, -1.0f, 0.0f); // Bottom Right
  VertexPositions[2] = glm::vec3(-1.0f, -1.0f, 0.0f); // Bottom Left
  VertexPositions[3] = glm::vec3(-1.0f,  1.0f, 0.0f); // Top Left

  // Color Data
  VertexColors.resize(VERTEX_COUNT);
  for (int i = 0; i < VERTEX_COUNT; ++i)
  {
    VertexColors[i] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // blue
    // Temp
    //VertexPositions[i] *= 40.0f;
  }

  // Texture Data
  VertexTextureCoords.resize(VERTEX_COUNT);
  VertexTextureCoords[0] = glm::vec2(1.0f, 0.0f); // Bottom Right
  VertexTextureCoords[1] = glm::vec2(1.0f, -1.0f); // Top Right
  VertexTextureCoords[2] = glm::vec2(0.0f, -1.0f); // Top Left
  VertexTextureCoords[3] = glm::vec2(0.0f, 0.0f); // Bottom Left

  Indices.resize(6);
  // Triangle 1
  Indices[0] = 0;
  Indices[1] = 3;
  Indices[2] = 2;
  // Triangle 2
  Indices[3] = 2;
  Indices[4] = 1;
  Indices[5] = 0;
}

Mesh::~Mesh(){}


size_t Mesh::GetIndexCount()
{
  return Indices.size();
}

std::vector<glm::vec3>& Mesh::GetVertexPositions()
{
  return VertexPositions;
}

std::vector<glm::vec4>& Mesh::GetVertexColors()
{
  return VertexColors;
}

std::vector<glm::vec2>& Mesh::GetVertexTextureCoords()
{
  return VertexTextureCoords;
}

std::vector<unsigned int>& Mesh::GetIndices()
{
  return Indices;
}


