/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsComponent.cpp
 **********************************************************************************************************************/
#include "GraphicsComponent.h"

Mesh::Mesh()
{
  int VertexCount = 4;
  // Position Data
  VertexPosition.resize(VertexCount);
  VertexPosition[0] = glm::vec3( 1.0f,  1.0f, 0.0f); // Top Right
  VertexPosition[1] = glm::vec3( 1.0f, -1.0f, 0.0f); // Bottom Right
  VertexPosition[2] = glm::vec3(-1.0f, -1.0f, 0.0f); // Bottom Left
  VertexPosition[3] = glm::vec3(-1.0f,  1.0f, 0.0f); // Top Left

  // Color Data
  VertexColor.resize(VertexCount);
  for (int i = 0; i < VertexCount; ++i)
  {
    VertexColor[i] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f); // blue
    // Temp
    VertexPosition[i] *= 40.0f;
  }

  // Texture Data
  VertexTexture.resize(VertexCount);
  VertexTexture[0] = glm::vec2(1.0f, 0.0f); // Bottom Right
  VertexTexture[1] = glm::vec2(1.0f, -1.0f); // Top Right
  VertexTexture[2] = glm::vec2(0.0f, -1.0f); // Top Left
  VertexTexture[3] = glm::vec2(0.0f, 0.0f); // Bottom Left

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


