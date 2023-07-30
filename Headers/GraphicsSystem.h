/*******************************************************************************************************************//**
 * \file GraphicsSystem.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Header for the GraphicsSystem. 
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.h"
#include "GraphicsComponent.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "Camera.h"
#include "Entity.h"
#include "Random.h"
typedef class Engine;

/// System that contains all logic to update entities with graphics components
class GraphicsSystem
{
public:
  GraphicsSystem();
  GraphicsSystem(Engine* ptr);
  // TODO: remove global friend function for a friend class Engine
  friend void DestroySystem(GraphicsSystem* system);

  void Update(float dt, const EntityPtr& entities, int arraySize);
  void MoveCamera(double x, double y);
  void ScrollCamera(double x, double y);
  glm::vec4 ViewToWorldTransform(const glm::vec4& vec);

private:
  /// Store data used to create a VAO for rendering
  struct RenderData
  {
    GLuint PositionVBO;     //!< Address to the vertex buffer for position data
    GLuint ColorVBO;        //!< Address to the vertex buffer for color data
    GLuint TextureVBO;      //!< Address to the vertex buffer for texture uv data
    GLuint TextureIndexVBO; //!< Address to the vertex buffer for texture index data

    GLuint VAO;             //!< Address to the vertex array
    GLuint EBO;             //!< Address to the element buffer

    GLuint TextureID;       //!< Address to the used texture
    GLuint ShaderID;        //!< Address to the used shader program
  };

  /// Exparimental struct
  struct VAOData
  {
    RenderData base;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> UVs;
    std::vector<unsigned int> TextureIndices;
    std::vector<unsigned int> indices;
  };

  Random PRNG;              //!< Random number generator, independent of others
  size_t EntityCount = 0;   //!< The number of entities in the last render
  Camera Viewport;          //!< Object that represents the window into the world
  GLFWwindow* Window;       //!< Handle on the window we are rendering to
  RenderData GeometryData;  //!< Collection of GPU memory address
  glm::mat4 ScreenBase;     //!< Transform screen coords into NDC
  Engine* Parent;           //!< Pointer to the Engine that manages this system

  ~GraphicsSystem();
  void Init();
  void Shutdown();
};
