/*******************************************************************************************************************//**
 * \file GraphicsSystem.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Implimentation for the Graphics System
 **********************************************************************************************************************/
#include <iostream>
#include <string>

#include "GraphicsSystem.h"
#include "GameWindow.h"
#include "Input.h"


Camera GraphicsSystem::Viewport;


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
GraphicsSystem::GraphicsSystem()
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
void GraphicsSystem::Update(float dt, const std::vector<EntityPtr>& entities)
{
  // Remove anything drawn to the last buffer
  glClear(GL_COLOR_BUFFER_BIT);

  size_t Size = entities.size();

  // If the data in our entities has changed we need to update our render data
  if (Size != EntityCount)
  {
    EntityCount = Size;

    BatchPositions.resize(EntityCount * VERTEX_COUNT);
    BatchColors.resize(EntityCount * VERTEX_COUNT);
    BatchTextureUVs.resize(EntityCount * VERTEX_COUNT);
    BatchIndices.resize(EntityCount * INDEX_COUNT);


    for (int i = 0; i < EntityCount; ++i)
    {
      BatchPrepare(entities[i]->GetGraphicsComponent(), entities[i]->GetTransformComponent()->GetModelMatrix(), i);
    }

    VAOPrepare(BatchPositions, BatchColors, BatchTextureUVs, BatchIndices);
  }

  GLuint ProgramID = ShManager.GetShaderID(entities[0]->GetGraphicsComponent()->GetShaderType());

  // Tell OpenGL the shader we are using
  glUseProgram(ProgramID);

  // Get the View matrix
  glm::mat4 Matrix = Viewport.GetViewMatrix();
  // Send the matrix combination to the shader
  glUniformMatrix4fv(glGetUniformLocation(ProgramID, "MVP"), 1, GL_FALSE, &Matrix[0][0]);


  glBindTexture(GL_TEXTURE_2D, TxManager.GetTextureID(entities[0]->GetGraphicsComponent()->GetTextureType()));

  // Bind the Vertex array for the current entity
  glBindVertexArray(GeometryData.VAO);
  glDrawElements(GL_TRIANGLES, BatchIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glfwSwapBuffers(Window);
  glfwPollEvents();
}


/*!
 * \brief Given a value we determine how the camera should move.
 *
 * \param Case
 *  Value needed to determine the required action for the camera.
 */
void GraphicsSystem::MoveCamera(int Case)
{
  // Current values of the camera
  glm::vec3 pos = Viewport.GetPosition();
  glm::vec3 scale = Viewport.GetScale();
  float angle = Viewport.GetRotation();

  /* Convert the angle to radians and change the perception of the angle so that it will appear like the camera is 
   * moving. We could get a similar effect without the 180 - X if we flipped the sign when adding the RotationOffset
   * to pos. */
  float rads = glm::radians(180 - angle);

  /* Create a rotation matrix to calculate the anti rotation of the camera so that when we move the camera the 
   * keyboard inputs will be relative to the monitor rather than the world orientation. */
  glm::mat2 RotationMat(glm::cos(rads), glm::sin(rads), -(glm::sin(rads)), glm::cos(rads));

  // Use the given value to determine how the camera should react
  switch (Case)
  {
  case UP:
  {
    // We want to move up relative to the monitor so we need a vector with a Y component for the rotation matrix.
    glm::vec2 RotateOffset(0.0f, scale.y / (DEFAULT_SCALE));
    RotateOffset = RotationMat * RotateOffset;
    pos += glm::vec3(RotateOffset, 0.0f);
    break;
  }
  case DOWN:
  {
    // We want to move up relative to the monitor so we need a vector with a Y component for the rotation matrix.
    glm::vec2 RotateOffset(0.0f, scale.y / (DEFAULT_SCALE));
    RotateOffset = RotationMat * RotateOffset;
    pos -= glm::vec3(RotateOffset, 0.0f);
    break;
  }
  case LEFT:
  {
    // We want to move up relative to the monitor so we need a vector with a X component for the rotation matrix.
    glm::vec2 RotateOffset(scale.x / (DEFAULT_SCALE), 0.0f);
    RotateOffset = RotationMat * RotateOffset;
    pos -= glm::vec3(RotateOffset, 0.0f);
    break;
  }
  case RIGHT:
  {
    // We want to move up relative to the monitor so we need a vector with a X component for the rotation matrix.
    glm::vec2 RotateOffset(scale.x / (DEFAULT_SCALE), 0.0f);
    RotateOffset = RotationMat * RotateOffset;
    pos += glm::vec3(RotateOffset, 0.0f);
    break;
  }
  case BACK:
    scale *= 1.1f;
    break;
  case FORWARD:
    scale *= 0.9f;
    break;
  case ROTATE_CCW:
    angle += 5.0f;
    break;
  case ROTATE_CW:
    angle -= 5.0f;
    break;
  default:
    return;
  }
  
  // Set all of the camera variables in case any were changed.
  Viewport.SetPosition(pos);
  Viewport.SetScale(scale);
  Viewport.SetRotation(angle);
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

  glfwSetKeyCallback(Window, KeyboardInputCallback);

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

  // Shader Manager
  ShManager.Init();
  TxManager.Init();

  glGenVertexArrays(1, &GeometryData.VAO);
  glGenBuffers(1, &GeometryData.PositionVBO);
  glGenBuffers(1, &GeometryData.ColorVBO);
  glGenBuffers(1, &GeometryData.TextureVBO);
  glGenBuffers(1, &GeometryData.EBO);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


/*!
 * \brief Shutsdown the Graphics system and delets any GPU memory and allocated variables
 *
 */
void GraphicsSystem::Shutdown()
{
  glDeleteBuffers(1, &GeometryData.PositionVBO);
  glDeleteBuffers(1, &GeometryData.ColorVBO);
  glDeleteBuffers(1, &GeometryData.TextureVBO);
  glDeleteBuffers(1, &GeometryData.EBO);
  glDeleteVertexArrays(1, &GeometryData.VAO);

  //TODO [2]: Create a shutdown function for the Texture and Shader managers
  glDeleteProgram(ShManager.GetShaderID(Shader::ShaderType::Basic_s));
}


/*!
 * \brief Attaches all neccessary information needed to draw a mesh to the Geometry VAO
 *
 * \param comp
 *  A pointer to a graphics component which contains all the information needed to draw the entity
 *
 */
void GraphicsSystem::VAOPrepare(GraphicsComponent* comp)
{
  Mesh* mesh = comp->GetMesh();

  glBindVertexArray(GeometryData.VAO);

  // Build a buffer for the position data for the VAO
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.PositionVBO);
  glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(glm::vec3), &mesh->GetVertexPositions()[0], GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // Build a buffer for the Color data for the VAO
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.ColorVBO);
  glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(glm::vec4), &mesh->GetVertexColors()[0], GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

  //TODO [4]: Add textureUV's to the old VAOPrepare function.

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeometryData.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_COUNT * sizeof(unsigned int), &mesh->GetIndices()[0], GL_DYNAMIC_DRAW);

  glBindVertexArray(0);
}


/*!
 * \brief Attaches all neccessary information needed to draw a mesh to the Geometry VAO
 *
 * \param comp
 *  A pointer to a graphics component which contains all the information needed to draw the entity
 *
 */
void GraphicsSystem::VAOPrepare(const std::vector<glm::vec3>& positions, const std::vector<glm::vec4>& colors, 
                                const std::vector<glm::vec2>& textureUV, const std::vector<unsigned int>& indicies)
{
  glBindVertexArray(GeometryData.VAO);

  // Build a buffer for the position data for the VAO
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.PositionVBO);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Build a buffer for the Color data for the VAO
  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.ColorVBO);
  glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, GeometryData.TextureVBO);
  glBufferData(GL_ARRAY_BUFFER, textureUV.size() * sizeof(glm::vec2), &textureUV[0], GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GeometryData.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_DYNAMIC_DRAW);

  glBindVertexArray(0);
}


/*!
 * \brief Adds the data from the given component to the graphics system for a batch render
 *
 * \param comp
 *  The graphics component we are adding to the batch render pass
 */
void GraphicsSystem::BatchPrepare(GraphicsComponent* comp, const glm::mat4& modleMatrix, int index)
{
  Mesh* ComponentMesh = comp->GetMesh();

  // Calculate the world positon
  glm::mat4 PositionComposit = modleMatrix * ComponentMesh->GetVertexPositions();
  // Store the world positon data
  for (int i = 0; i < VERTEX_COUNT; ++i)
  {
    BatchPositions[index * VERTEX_COUNT + i] = PositionComposit[i];
  }

  // Store the color and texture UV data
  for (int i = 0; i < VERTEX_COUNT; ++i)
  {
    int BatchIndex = index * VERTEX_COUNT + i;

    BatchColors[BatchIndex] = ComponentMesh->GetVertexColors()[i];
    BatchTextureUVs[BatchIndex] = ComponentMesh->GetVertexTextureCoords()[i];
  }

  // Get the generic index values
  std::vector<unsigned int> tempind = ComponentMesh->GetIndices();
  // Store the new indices into the composit mesh data vectors
  for (int i = 0; i < INDEX_COUNT; ++i)
  {
    // Clalculate the new index
    BatchIndices[index * INDEX_COUNT + i] = tempind[i] + (index * VERTEX_COUNT);
  }
}
