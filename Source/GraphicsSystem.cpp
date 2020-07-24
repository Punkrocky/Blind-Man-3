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
#include "Input.h"

Camera GraphicsSystem::Viewport;

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
  //mesh = new Mesh;
  this->Init();
}

void DestroySystem(GraphicsSystem* system)
{
  delete system;
}

static float Tdt;

void GraphicsSystem::Update(float dt, std::vector<EntityPtr> entities)
{
  // Remove anything drawn to the last buffer
  glClear(GL_COLOR_BUFFER_BIT);

  Tdt += dt;
  size_t Size = entities.size();

  for (int i = 0; i < Size; ++i)
  {
    VAOPrepare(entities[i]->GetGraphicsComponent());

    // Tell OpenGL the shader we are using
    glUseProgram(ShManager.GetShaderID(0));

    // Get the View matrix
    glm::mat4 Matrix = Viewport.GetViewMatrix();
    // Combine the view matrix and the modle matrix together
    Matrix *= entities[i]->GetTransformComponent()->GetModelMatrix();
    // Send the matrix combination to the shader
    glUniformMatrix4fv(glGetUniformLocation(ShManager.GetShaderID(0), "MVP"), 1, GL_FALSE, &Matrix[0][0]);

    // Bind the Vertex array for the current entity
    glBindVertexArray(GeometryData.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  glfwSwapBuffers(Window);
  glfwPollEvents();
}

// TODO: comment this shit
void GraphicsSystem::MoveCamera(int n)
{
  glm::vec3 pos = Viewport.GetPosition();
  glm::vec3 scale = Viewport.GetScale();
  float angle = Viewport.GetRotation();

  glm::vec3 POS(0.0f, 0.0f, 0.0f);
  glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f), glm::radians(180-angle), glm::vec3(0.0f, 0.0f, 1.0f));


  switch (n)
  {
  case UP:
    POS.y += DEFAULT_SCALE;
    POS = Rotation * glm::vec4(POS, 1.0f);
    pos += POS;
    break;
  case DOWN:
    POS.y += DEFAULT_SCALE;
    POS = Rotation * glm::vec4(POS, 1.0f);
    pos -= POS;
    break;
  case LEFT:
    POS.x += DEFAULT_SCALE;
    POS = Rotation * glm::vec4(POS, 1.0f);
    pos -= POS;
    break;
  case RIGHT:
    POS.x += DEFAULT_SCALE;
    POS = Rotation * glm::vec4(POS, 1.0f);
    pos += POS;
    break;
  case BACK:
    scale *= 1.1f;
    break;
  case FORWARD:
    scale *= 0.9f;
    break;
  case ROTATE_CCW:
    angle -= 10.0f;
    break;
  case ROTATE_CW:
    angle += 10.0f;
    break;
  default:
    return;
  }
  
  Viewport.SetPosition(pos);
  Viewport.SetScale(scale);
  Viewport.SetRotation(angle);
}


/*****************************| PRIVATE MEMBERS |*****************************/

GraphicsSystem::~GraphicsSystem()
{
  this->Shutdown();
}

void GraphicsSystem::Init()
{
  Window = GameWindow::GetPtrGameWindow();

  glm::ivec3 WindowSize(1);
  glfwGetWindowSize(Window, &WindowSize.x, &WindowSize.y);
  Viewport.SetScale(WindowSize);

  glfwSetKeyCallback(Window, KeyboardInputCallback);

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

  glGenVertexArrays(1, &GeometryData.VAO);
  glGenBuffers(1, &GeometryData.PositionVBO);
  glGenBuffers(1, &GeometryData.ColorVBO);
  glGenBuffers(1, &GeometryData.TextureVBO);
  glGenBuffers(1, &GeometryData.EBO);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsSystem::Shutdown()
{
  glDeleteBuffers(1, &GeometryData.PositionVBO);
  glDeleteBuffers(1, &GeometryData.ColorVBO);
  glDeleteBuffers(1, &GeometryData.TextureVBO);
  glDeleteBuffers(1, &GeometryData.EBO);
  glDeleteVertexArrays(1, &GeometryData.VAO);

  glDeleteProgram(ShManager.GetShaderID(0));
}

void GraphicsSystem::VAOPrepare(GraphicsComponent* comp)
{
  Mesh* mesh = comp->GetMesh();

  glBindVertexArray(GeometryData.VAO);

  // Build a buffer for the position data for the VAO
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.PositionVBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->GetVertexCount() * sizeof(glm::vec3), &mesh->GetVertexPositions()[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // Build a buffer for the Color data for the VAO
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.ColorVBO);
  glBufferData(GL_ARRAY_BUFFER, mesh->GetVertexCount() * sizeof(glm::vec4), &mesh->GetVertexColors()[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeometryData.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->GetIndexCount() * sizeof(unsigned int), &mesh->GetIndices()[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
}
