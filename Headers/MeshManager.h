/*******************************************************************************************************************//**
 * \file MeshManager.h
 * \author Matthew LaDouceur
 * \date 8-5-2020
 * \brief Header for the MeshManager
 **********************************************************************************************************************/
#pragma once
#include <vector>

#include "GraphicsComponent.h"

/// Holds a basic mesh object to be used by all entities. Removes the need for each entity to have a mesh object.
class MeshManager
{
public:
  MeshManager();
  ~MeshManager() = default;

  const Mesh& GetMeshObject() const;

private:
  Mesh SquareMesh; //!< Basic square mesh every entity uses
};
