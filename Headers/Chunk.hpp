/*******************************************************************************************************************//**
 * \file Chunk.hpp
 * \author Matthew LaDouceur
 * \date 11-8-2023
 * \brief Source for a hierarchical chunking system
 **********************************************************************************************************************/
#pragma once
#include <vector>
#include <array>

#include "OpenGLIncludes.hpp"
#include "GlobalDefines.h"
#include "Entity.hpp"


class Chunk
{
public:
  Chunk();
  ~Chunk() = default;
  void InitChunkData();
  void Draw(float dt, const glm::mat4& viewMatrix) const;

  const unsigned int ID;
  std::array<EntityPtr, TILES_PER_CHUNK_SQRD> ChunkEntities;
private:
  GLuint ChunkVAO; //!< Vertex array object of the chunk
};
