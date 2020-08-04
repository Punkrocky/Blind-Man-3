/*******************************************************************************************************************//**
 * \file GraphicsSystem.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Header for the Graphics System. 
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.h"
#include "GraphicsComponent.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Entity.h"

/// System that contains all logic to update entities with graphics components
class GraphicsSystem
{
public:

  GraphicsSystem();
  friend void DestroySystem(GraphicsSystem* system);

  void Update(float dt, const std::vector<EntityPtr>& entities);
  static void MoveCamera(int n);

private:
  /// Store data used to create a VAO for rendering
  struct RenderData
  {
    GLuint PositionVBO; //!< Address to the vertex buffer for position data
    GLuint ColorVBO;    //!< Address to the vertex buffer for color data
    GLuint TextureVBO;  //!< Address to the bertex buffer for texture uv data
    GLuint VAO;         //!< Address to the vertex array
    GLuint EBO;         //!< Address to the element buffer

    GLuint TextureID;   //!< Address to the used texture
    GLuint ShaderID;    //!< Address to the used shader program
  };

  /// Exparimental struct
  struct VAOData
  {
    RenderData base;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> UVs;
    std::vector<unsigned int> indices;
  };
  size_t EntityCount = 0;   //!< The number of entities in the last render

  static Camera Viewport;   //!< Object that represents the window into the world
  GLFWwindow* Window;       //!< Handle on the window we are rendering to
  TextureManager TxManager; //!< Texture Manager
  ShaderManager ShManager;  //!< Shader Manager
  RenderData GeometryData;  //!< Collection of GPU memory address


  std::vector<glm::vec3> BatchPositions;  //!< Hold all the world space position data for a render pass
  std::vector<glm::vec4> BatchColors;     //!< Hold all the color data for a render pass
  std::vector<glm::vec2> BatchTextureUVs; //!< Hold all the texture uv data for a render pass
  std::vector<unsigned int> BatchIndices; //!< Hold all the indices into the above arrays of data


  ~GraphicsSystem();
  void Init();
  void Shutdown();

  void VAOPrepare(GraphicsComponent* comp);
  void VAOPrepare(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors, 
                  const std::vector<glm::vec2>& textureUV, const std::vector<unsigned int>& indicies);

  void BatchPrepare(GraphicsComponent* comp, const glm::mat4& modleMatrix, int index);
};
