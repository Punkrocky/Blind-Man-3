/*******************************************************************************************************************//**
 * \file MeshManager.cpp
 * \author Matthew LaDouceur
 * \date 8-5-2020
 * \brief Source for the MeshManager
 **********************************************************************************************************************/
#include "MeshManager.h"


 /*!
  * \brief Creates the base mesh object used for all entities. Does not dynamically allocate the mesh.
  *
  */
MeshManager::MeshManager()
{
  SquareMesh = Mesh();
}

/*!
 * \brief Gives the mesh object on request. Does not alter the state of the class
 *
 * \return A constant refrence to the mesh object so it may not be modified
 *
 */
const Mesh& MeshManager::GetMeshObject() const
{
  return SquareMesh;
}

