/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsSystem.h
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.h"
#include "GraphicsComponent.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Entity.h"

class GraphicsSystem
{
public:
  /**
   * @brief Default constructor for the Graphics System
   *
   */
  GraphicsSystem();
  /**
   * @brief Explicit destructor for the graphics system to dissuade people from accidentaly makeing new Graphics systems
   *
   * @param system
   *  Pointer the the graphics system that we are deconstructing
   */
  friend void DestroySystem(GraphicsSystem* system);

  /**
   * @brief Update the graphics system
   *
   * @param dt  The delta time that has passed since the last frame
   *
   * @param entities  A vector of entity pointers that need to be drawn to the screen this frame
   */
  void Update(float dt, std::vector<EntityPtr> entities);

  /**
   * @brief Given a value we determine how the camera should move.
   *
   * @param Case
   *  Value needed to determine the required action for the camera.
   */
  static void MoveCamera(int n);


private:
  /**
   * @brief Initalizer for the Graphics system and OpenGL
   *
   */
  void Init();

  /**
   * @brief Shutsdown the Graphics system and delets any GPU memory and allocated variables
   *
   */
  void Shutdown();

  /**
   * @brief Private destructor will make the class only dynamically allocatable.
   *
   */
  ~GraphicsSystem();

  /**
   * @brief Attaches all neccessary information needed to draw a mesh to the Geometry VAO
   *
   * @param comp
   *  A pointer to a graphics component which contains all the information needed to draw the entity
   *
   */
  void VAOPrepare(GraphicsComponent* comp);
  void VAOPrepare(std::vector<glm::vec3> positions, std::vector<glm::vec4> colors, std::vector<unsigned int> indicies);

  /**
   * @brief Adds the data from the given component to the graphics system for a batch render
   *
   * @param comp  The graphics component we are adding to the batch render pass
   */
  void BatchPrepare(GraphicsComponent* comp, glm::mat4 modleMatrix, int index);

  struct RenderData
  {
    GLuint PositionVBO;
    GLuint ColorVBO;
    GLuint TextureVBO;
    GLuint VAO;
    GLuint EBO;

    // GLuint TextureID;
    // GLuint ShaderID;
  };
  RenderData GeometryData;

  GLFWwindow* Window;
  ShaderManager ShManager;
  static Camera Viewport;

  std::vector<glm::vec3> BatchPositions;
  std::vector<glm::vec4> BatchColors;
  std::vector<unsigned int> BatchIndices;
};
