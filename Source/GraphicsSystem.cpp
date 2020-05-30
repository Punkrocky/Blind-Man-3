/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsSystem.cpp
 **********************************************************************************************************************/
#include <iostream>
#include <string>

#include "GraphicsSystem.h"
#include "GameWindow.h"


void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                              const GLchar* message, const void* userParam)
{
  std::string Type, Severity;

  switch (severity)
  {
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    return;
    Severity = "NOTIFICATION ";
    break;
  case GL_DEBUG_SEVERITY_LOW:
    Severity = "LOW ";
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    Severity = "MEDIUM ";
    break;
  case GL_DEBUG_SEVERITY_HIGH:
    Severity = "HIGH ";
    break;
  default:
    Severity = "Not caught in switch statement! ";
    break;
  }

  switch (type)
  {
  case GL_DEBUG_TYPE_ERROR:
    Type = " ** GL ERROR ** ";
    break;
  case GL_DEBUG_TYPE_MARKER:
    Type = "GL MARKER ";
    break;
  case GL_DEBUG_TYPE_OTHER:
    Type = "GL OTHER ";
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    Type = "GL PERFORMANCE ";
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    Type = "GL UNDEFINED BEHAVIOR ";
    break;
  default:
    Type = "Not caught in switch statement! ";
    break;
  }
  std::string ErrorMsg = "\n GL CALLBACK  Type: " + Type + "Severity: " + Severity + "Message: " + message + "\n\n";

  //TraceMessage(ErrorMsg.c_str(), __FILE__, __LINE__);
  std::cerr << "\n GL CALLBACK: "
            << "Type: " << Type.c_str() << "Severity: " << Severity.c_str() << "Message: " << message << "\n\n";
}

GraphicsSystem::GraphicsSystem()
{
  mesh = new Mesh;
  this->Init();
}

void DestroySystem(GraphicsSystem* system)
{
  delete system;
}


void GraphicsSystem::Update()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(ShManager.GetShaderID(0));

  glm::mat4 Matrix = Viewport.GetViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(ShManager.GetShaderID(0), "MVP"), 1, GL_FALSE, &Matrix[0][0]);

  glBindVertexArray(GeometryData.VAO);
  //glDrawArrays(GL_TRIANGLES, 0, 6);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glfwSwapBuffers(Window);
  glfwPollEvents();
}



/*****************************| PRIVATE MEMBERS |*****************************/

GraphicsSystem::~GraphicsSystem()
{
  this->Shutdown();
  delete mesh;
}

void GraphicsSystem::Init()
{
  Window = GameWindow::GetPtrGameWindow();

  glm::ivec3 WindowSize(1);
  glfwGetWindowSize(Window, &WindowSize.x, &WindowSize.y);
  Viewport.SetScale(WindowSize);

  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Failed to init GLEW\n";
    glfwTerminate();
    return;
  }

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable the use of debug messages from opengl
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(DebugCallback, 0);

  // Shader Manager
  ShManager.Init();
  // Texture Manager
  // TODO: Texture Manager

  // VAO for post effects
  glGenVertexArrays(1, &GeometryData.VAO);
  glBindVertexArray(GeometryData.VAO);

  // Build a buffer for the position data for the VAO
  glGenBuffers(1, &GeometryData.PositionVBO);
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.PositionVBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->VertexPosition.size() * sizeof(glm::vec3), &mesh->VertexPosition[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // Build a buffer for the Color data for the VAO
  glGenBuffers(1, &GeometryData.ColorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.ColorVBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->VertexColor.size() * sizeof(glm::vec4), &mesh->VertexColor[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &GeometryData.EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeometryData.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Indices.size() * sizeof(unsigned int), &mesh->Indices[0], GL_STATIC_DRAW);

  glBindVertexArray(0);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsSystem::Shutdown()
{
  glDeleteVertexArrays(1, &GeometryData.VAO);
  glDeleteBuffers(1, &GeometryData.PositionVBO);
  glDeleteBuffers(1, &GeometryData.ColorVBO);
  glDeleteProgram(ShManager.GetShaderID(0));
}
