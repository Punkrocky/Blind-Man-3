/*******************************************************************************************************************//**
 * \file GraphicsSystem.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Source for the GraphicsSystem
 **********************************************************************************************************************/
#include <iostream>
#include <string>

#include "GraphicsSystem.h"
#include "GameWindow.h"
#include "Engine.h"
#include "Input.h"



/*!
 * \brief Prints OpenGL debugging info and error messages to the consle
 *
 * \param source
 *  describing where the message came from
 *
 * \param type
 *  describing the type of message
 *
 * \param id
 *  value of the message id
 *
 * \param severity
 *  describing the severity of the message
 *
 * \param length
 *  the length of the message being sent
 *
 * \param message
 *  the message being sent
 *
 * \param userParam
 *  an empty parameter for any extra user data
 *
 */
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


/*!
 * \brief Default constructor for the Graphics System
 *
 */
GraphicsSystem::GraphicsSystem() : PRNG(1200), ScreenBase(0.0f)
{
  this->Init();
}


GraphicsSystem::GraphicsSystem(Engine* ptr) : Parent(ptr), PRNG(1200), ScreenBase(0.0f)
{
  this->Init();
}

/*!
 * \brief Explicit destructor for the graphics system to dissuade people from accidentaly makeing new Graphics systems
 *
 * \param system
 *  Pointer the the graphics system that we are deconstructing
 */
void DestroySystem(GraphicsSystem* system)
{
  delete system;
}


/*!
 * \brief Update the graphics system
 *
 * \param dt
 *  The delta time that has passed since the last frame
 *
 * \param entities
 *  A vector of entity pointers that need to be drawn to the screen this frame
 */
void GraphicsSystem::Update(float dt, const EntityPtr& entities, int arraySize)
{
  DebugTimer.StartFrame();

  // Remove anything drawn to the last buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // Get the View matrix
  glm::mat4 ViewMatrix = Viewport.GetViewMatrix();

  for (int i = 0; i < arraySize; ++i)
  {
    entities[i].Draw(ViewMatrix);
  }
  glm::vec4 LineColor(1.0f, 0.0f, 0.0f, 1.0f); // Red
  glm::mat4 InverseMatrix = glm::inverse(ViewMatrix);
  glm::vec3 ViewPortScale = Viewport.GetScale();
  ViewPortScale.y = -ViewPortScale.y;

  // Workaround for not having an entity, works because .json is ordered specifically
  GLuint ShaderID = 4;
  glUseProgram(ShaderID);
  glUniformMatrix4fv(glGetUniformLocation(ShaderID, "P"), 1, GL_FALSE, &ViewMatrix[0][0]);
  glUniform4fv(glGetUniformLocation(ShaderID, "fColor"), 1, &LineColor[0]);

  // Get the mouse screen position and transform it to NDC then to worldspace
  glm::vec4 MousePos = InverseMatrix * ScreenBase * glm::vec4(GetLastMousePos(), 0.0f, 1.0f);
  // Transform the NDC position that represents the center, to worldspace
  glm::vec4 ScreenCenter = InverseMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

  glm::mat4 mMat = Parent->GetEntityAtCoords(Parent->GetTileCoords(MousePos)).GetTransformComponent()->GetModelMatrix();
  glm::mat4 wPos;
  wPos[0] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);   // Top Right
  wPos[1] = glm::vec4(1.0f, -1.0f, 0.0f, 1.0f);  // Bottom Right
  wPos[2] = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // Bottom Left
  wPos[3] = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);  // Top Left
  wPos = mMat * wPos;

  const int pSize = 12;
  glm::vec4 Points[pSize] = { glm::vec4(0.0f) };
  // Line 0
  Points[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  Points[1] = MousePos;
  // Line 1
  Points[2] = wPos[0];
  Points[3] = wPos[1];
  // Line 2
  Points[4] = wPos[1];
  Points[5] = wPos[2];
  // Line 3
  Points[6] = wPos[2];
  Points[7] = wPos[3];
  // Line 4
  Points[8] = wPos[3];
  Points[9] = wPos[0];
  // Line 5
  Points[10] = ScreenCenter;
  Points[11] = MousePos;

  glBindVertexArray(GeometryData.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.PositionVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Points), &Points[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glDrawArrays(GL_LINES, 0, pSize);

  glUseProgram(0);
  glBindVertexArray(0);

  glfwSwapBuffers(Window);
  glfwPollEvents();

  DebugTimer.EndFrame();
  std::cout << " - Graphics " << DebugTimer;
}


/*!
 * \brief Given a value we determine how the camera should move
 *
 * \param Case
 *  Value needed to determine the required action for the camera
 */
void GraphicsSystem::MoveCamera(double x, double y)
{
  // Current values of the camera
  glm::vec3 pos = Viewport.GetPosition();
  float scale = 1.0f / Viewport.GetScale().z;
  float radsAngle = glm::radians(Viewport.GetRotation());
  glm::mat4 RotMat = glm::rotate(glm::mat4(1.0f), radsAngle, glm::vec3(0, 0, -1));

  // Modify the position based on any change to the viewport scale(zooming)
  pos += glm::vec3(RotMat * (glm::vec4(x, y, -1, 1.0f) / scale));
  pos.z = -1.0f;
  
  // Apply changes to the viewport
  Viewport.SetPosition(pos);
}


void GraphicsSystem::ScrollCamera(double x, double y)
{
  glm::vec3 scale = Viewport.GetScale();  // size of the viewport in the world
  glm::vec3 pos = Viewport.GetPosition(); // Current position of the viewport
  glm::mat4 InvViewMat = glm::inverse(Viewport.GetViewMatrix());

  // Camera zoom to mouse position.
  glm::vec4 mouseWorldPos = InvViewMat * ScreenBase * glm::vec4(GetLastMousePos(), 0.0f, 1.0f);
  glm::vec4 screenCenter = InvViewMat * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 mouseWorldVec = (mouseWorldPos - screenCenter) * static_cast<float>(y);

  pos += glm::vec3(mouseWorldVec);
  scale *= (1.0 + y);

  Viewport.SetScale(scale);
  Viewport.SetPosition(pos);
}



glm::vec4 GraphicsSystem::ViewToWorldTransform(const glm::vec4& vec)
{
  return glm::inverse(Viewport.GetViewMatrix()) * ScreenBase * vec;
}

/**********************************************************************************************************************\
|*************************************************| PRIVATE MEMBERS |**************************************************|
\**********************************************************************************************************************/

/*!
  * \brief Private destructor will make the class only dynamically allocatable.
  *
  */
GraphicsSystem::~GraphicsSystem()
{
  this->Shutdown();
}

/*!
 * \brief Initalizer for the Graphics system and OpenGL
 *
 */
void GraphicsSystem::Init()
{
  Window = GameWindow::GetPtrGameWindow();

  glm::ivec3 WindowSize(1);
  glfwGetWindowSize(Window, &WindowSize.x, &WindowSize.y);
  Viewport.SetScale(WindowSize);

  // Input callback functions
  glfwSetKeyCallback(Window, KeyboardInputCallback);
  glfwSetMouseButtonCallback(Window, MouseButtonCallback);
  glfwSetCursorPosCallback(Window, MouseCallback);
  glfwSetScrollCallback(Window, MouseScrollCallback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Failed to init GLEW\n";
    glfwTerminate();
    return;
  }

  // Set the clear color to a nice blue-gray background color
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable the use of debug messages from opengl
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(DebugCallback, 0);

  glGenVertexArrays(1, &GeometryData.VAO);
  glGenBuffers(1, &GeometryData.PositionVBO);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  ScreenBase[0] = glm::vec4(2.0f / WindowSize.x, 0.0f, 0.0f, 0.0f);
  ScreenBase[1] = glm::vec4(0.0f, -2.0f / WindowSize.y, 0.0f, 0.0f);
  ScreenBase[3] = glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f);

  std::cout << "Graphics System Constructed\n";
}


/*!
 * \brief Shutsdown the Graphics system and delets any GPU memory and allocated variables
 *
 */
void GraphicsSystem::Shutdown()
{
  //TODO [2]: Create a shutdown function for the Texture, Shader, and Mesh managers
  //glDeleteProgram(ShManager->GetShaderID(Shader::ShaderType::Basic_s));

  
}
