/*******************************************************************************************************************//**
 * \file MeshManager.cpp
 * \author Matthew LaDouceur
 * \date 8-5-2020
 * \brief Source for the MeshManager
 **********************************************************************************************************************/
#include "MeshManager.hpp"

Mesh::Mesh(MeshType type) : Type(type)
{
  // Position Data
  glm::mat4 VertexPositions;
  VertexPositions[0] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f); // Top Right
  VertexPositions[1] = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f); // Bottom Right
  VertexPositions[2] = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // Bottom Left
  VertexPositions[3] = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f); // Top Left

  // Texture Data
  glm::mat4x2 VertexTextureCoords;
  VertexTextureCoords[0] = glm::vec2(1.0f, 0.0f); // Bottom Right
  VertexTextureCoords[1] = glm::vec2(1.0f, 1.0f); // Top Right
  VertexTextureCoords[2] = glm::vec2(0.0f, 1.0f); // Top Left
  VertexTextureCoords[3] = glm::vec2(0.0f, 0.0f); // Bottom Left

  unsigned int Indices[6];
  // Triangle 1
  Indices[0] = 0;
  Indices[1] = 3;
  Indices[2] = 2;
  // Triangle 2
  Indices[3] = 2;
  Indices[4] = 1;
  Indices[5] = 0;

  GLuint EBO = 0;
  GLuint PosVBO = 0;
  GLuint TextureVBO = 0;

  // Generate Buffers and Arrays
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &PosVBO);
  glGenBuffers(1, &TextureVBO);

  // Bind and enable attributes of vertex array
  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);

  // Bind the position data
  glBindBuffer(GL_ARRAY_BUFFER, PosVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &VertexPositions[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Bind textures UV data
  glBindBuffer(GL_ARRAY_BUFFER, TextureVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4x2), &VertexTextureCoords[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // BInd the index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
  
  // Unbind buffers and arrays 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &PosVBO);
  glDeleteBuffers(1, &TextureVBO);
}


Mesh::Mesh(const Mesh& rhs)
{
  this->Type = rhs.Type;
  this->VAO = rhs.VAO;
}


GLuint Mesh::GetVertexArray()
{
  return VAO;
}


 /*!
  * \brief Creates the base mesh object used for all entities. Does not dynamically allocate the mesh.
  *
  */
MeshManager::MeshManager()
{
  MeshArray.emplace_back(Mesh());
}

/*!
 * \brief Gives the mesh object on request. Does not alter the state of the class
 *
 * \return A constant refrence to the mesh object so it may not be modified
 *
 */
Mesh* MeshManager::GetMeshObject(Mesh::MeshType type)
{
  return &MeshArray[static_cast<int>(type)];
}

