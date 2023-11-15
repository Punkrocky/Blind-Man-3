/*******************************************************************************************************************//**
 * \file MeshManager.h
 * \author Matthew LaDouceur
 * \date 8-5-2020
 * \brief Header for the MeshManager
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.hpp"

 /// Data related to mesh objects
class Mesh
{
public:
  enum class MeshType
  {
    INVALID_MESH_TYPE = -1,
    Square_m,
    MESH_TYPE_TOTAL
  };

  Mesh(MeshType type = MeshType::Square_m);
  Mesh(const Mesh& rhs);
  ~Mesh() = default;

  void SetColor(const glm::vec4 color);

  GLuint GetVertexArray();

private:
  MeshType Type;
  GLuint VAO;
};

/// Holds a basic mesh object to be used by all entities. Removes the need for each entity to have a mesh object.
class MeshManager
{
public:
  MeshManager();
  ~MeshManager() = default;

  Mesh* GetMeshObject(Mesh::MeshType type);

private:
  std::vector<Mesh> MeshArray; //!< Basic square mesh every entity uses
};

typedef Mesh* MeshPtr;
typedef MeshManager* MeshManagerPtr;
typedef Mesh::MeshType MType;
