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
  GraphicsSystem();
  friend void DestroySystem(GraphicsSystem* system);

  void Update(float dt, Entity* entity);

private:
  void Init();
  void Shutdown();
  ~GraphicsSystem(); // Private destructor will make the class only dynamically allocatable.

  void VAOPrepare(GraphicsComponent* comp);

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
  Camera Viewport;
};
