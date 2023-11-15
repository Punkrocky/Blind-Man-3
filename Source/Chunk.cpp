/*******************************************************************************************************************//**
 * \file Chunk.cpp
 * \author Matthew LaDouceur
 * \date 11-8-2023
 * \brief Source for a hierarchical chunking system
 **********************************************************************************************************************/
#include "Chunk.hpp"

static unsigned int CHUNK_ID = 1;

Chunk::Chunk() : ID(CHUNK_ID++), ChunkVAO(0)
{}

void Chunk::InitChunkData()
{
  // Construct the vertex buffer data
  std::array<glm::mat4, TILES_PER_CHUNK_SQRD> Position_Data;  // Combined position data of each tile
  std::array<glm::mat4, TILES_PER_CHUNK_SQRD> Color_Data;     // Combined Color data of each tile
  std::array<glm::mat4x2, TILES_PER_CHUNK_SQRD> Texture_Data; // Combined Texture data of each tile

  // Color Data
  glm::mat4 VertexColors;
  VertexColors[0] = glm::vec4(1.0f); // White
  VertexColors[1] = glm::vec4(1.0f); // White
  VertexColors[2] = glm::vec4(1.0f); // White
  VertexColors[3] = glm::vec4(1.0f); // White

  // Texture Data
  glm::mat4x2 VertexTextureCoords;
  VertexTextureCoords[0] = glm::vec2(1.0f, 0.0f); // Bottom Right
  VertexTextureCoords[1] = glm::vec2(1.0f, 1.0f); // Top Right
  VertexTextureCoords[2] = glm::vec2(0.0f, 1.0f); // Top Left
  VertexTextureCoords[3] = glm::vec2(0.0f, 0.0f); // Bottom Left

  // Collect all vertex specific data
  size_t EntityCount = ChunkEntities.size();
  for (int i = 0; i < EntityCount; ++i)
  {
    glm::vec4 TempColor = ChunkEntities[i]->GetGraphicsComponent()->GetColor();
    glm::mat4 ModleMat = ChunkEntities[i]->GetTransformComponent()->GetModelMatrix();

    VertexColors[0] = TempColor;
    VertexColors[1] = TempColor;
    VertexColors[2] = TempColor;
    VertexColors[3] = TempColor;

    // Position Data
    glm::mat4 VertexPositions;
    VertexPositions[0] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);   // Top Right
    VertexPositions[1] = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);  // Bottom Right
    VertexPositions[2] = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // Bottom Left
    VertexPositions[3] = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);  // Top Left

    VertexPositions[0] = ModleMat * VertexPositions[0];
    VertexPositions[1] = ModleMat * VertexPositions[1];
    VertexPositions[2] = ModleMat * VertexPositions[2];
    VertexPositions[3] = ModleMat * VertexPositions[3];

    Position_Data[i] = VertexPositions;
    Color_Data[i] = VertexColors;
    Texture_Data[i] = VertexTextureCoords;
  }


  // Construct the element buffer data
  std::array<unsigned int, TILES_PER_CHUNK_SQRD * INDEX_COUNT> Element_Data; // Modified index elements into the vertex data

  unsigned int Indices[INDEX_COUNT];
  // Triangle 1
  Indices[0] = 0;
  Indices[1] = 3;
  Indices[2] = 2;
  // Triangle 2
  Indices[3] = 2;
  Indices[4] = 1;
  Indices[5] = 0;

  for (int i = 0; i < EntityCount; ++i)
  {
    for (int j = 0; j < INDEX_COUNT; ++j)
    {
      Element_Data[i * INDEX_COUNT + j] = Indices[j] + i * VERTEX_COUNT;
    }
  }


  // Begin building the vertex array object for rending
  GLuint EBO = 0;
  GLuint PosVBO = 0;
  GLuint ColorVBO = 0;
  GLuint TextureVBO = 0;

  // Clear the VAO if there is previous data there
  if (ChunkVAO != 0)
  {
    glDeleteVertexArrays(1, &ChunkVAO);
  }

  // Generate Buffers and Arrays
  glGenVertexArrays(1, &ChunkVAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &PosVBO);
  glGenBuffers(1, &ColorVBO);
  glGenBuffers(1, &TextureVBO);

  // Bind and enable attributes of vertex array
  glBindVertexArray(ChunkVAO);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  // Bind the position data
  glBindBuffer(GL_ARRAY_BUFFER, PosVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * TILES_PER_CHUNK_SQRD, &Position_Data[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Bind the color data
  glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * TILES_PER_CHUNK_SQRD, &Color_Data[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Bind textures UV data
  glBindBuffer(GL_ARRAY_BUFFER, TextureVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4x2) * TILES_PER_CHUNK_SQRD, &Texture_Data[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // BInd the index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int) * TILES_PER_CHUNK_SQRD, &Element_Data[0], GL_STATIC_DRAW);

  // Unbind buffers and arrays 
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &PosVBO);
  glDeleteBuffers(1, &ColorVBO);
  glDeleteBuffers(1, &TextureVBO);
}

void Chunk::Draw(float dt, const glm::mat4& viewMatrix) const
{
  GLuint ShaderID = ChunkEntities[0]->GetGraphicsComponent()->GetShader().GetShaderLocation();
  glUseProgram(ShaderID);
  glUniform1i(glGetUniformLocation(ShaderID, "Texture"), 0);
  //glBindTextureUnit(0, GraphicsComp->texture->GetTextureLocation());

  glUniformMatrix4fv(glGetUniformLocation(ShaderID, "P"), 1, GL_FALSE, &viewMatrix[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(ShaderID, "M"), 1, GL_FALSE, &glm::mat4(1.0f)[0][0]);
  //glUniform4fv(glGetUniformLocation(ShaderID, "fColor"), 1, &GraphicsComp->GetColor()[0]);
  //GraphicsComp->SetColor(GraphicsComp->Color);

  glBindVertexArray(ChunkVAO);
  glDrawElements(GL_TRIANGLES, TILES_PER_CHUNK_SQRD * INDEX_COUNT, GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);
}
