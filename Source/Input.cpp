/*******************************************************************************************************************//**
* \file Input.cpp
* \author Matthew LaDouceur
* \date 8-25-2022
* \brief Handle GLFW input
 **********************************************************************************************************************/
#include <iostream>

#include "OpenGLIncludes.hpp"
#include "GraphicsSystem.hpp"
#include "Input.hpp"

void (*DragCamera)(double, double);
void (*ZoomCamera)(double, double);
void (*Key_S)();
void (*Key_G)();
int (*GrabTileValue)();
void (*PlaceTileValue)(int);

double xPosDelta = 0.0;
double yPosDelta = 0.0;
double xPosLast = 0.0;
double yPosLast = 0.0;

int HeldValue = 0;

bool bLeftButton = false;
bool bRightButton = false;
bool bDragCamera = false;
bool bEscapePressed = false;

void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch (key)
  {
  case GLFW_KEY_ESCAPE:
  {
    if (action == GLFW_PRESS)
    {
      // Toggle the escape key state
      bEscapePressed = !bEscapePressed;
    }
  }
  break;
  case GLFW_KEY_S:
  {
    if (action == GLFW_PRESS)
    {
      Key_S();
    }
  }
  break;
  case GLFW_KEY_G:
  {
    if (action == GLFW_PRESS)
    {
      Key_G();
    }
  }
  break;
  default:
    break;
  }
}


void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  // Only update the camera posion if the left mouse button is held down
  if (bRightButton)
  {
    bDragCamera = true;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Calculate how much the mouse moved
    xPosDelta = xpos - xPosLast;
    yPosDelta = ypos - yPosLast;
    // Call the dedicated function pointer
    DragCamera(xPosDelta * 2, -(yPosDelta * 2));
  }

  // Always update the old position of the mouse
  xPosLast = xpos;
  yPosLast = ypos;
  //std::cout << "x:" << xPosLast << " Y:" << yPosLast << '\n';
}


void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  ZoomCamera(xoffset, -(yoffset * 0.1));
}


void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT)
  {
    if (action) // Pressed
    {
      bRightButton = true;
    }
    else // Released
    {
      bRightButton = false;
      if (!bDragCamera)
      {
        HeldValue = GrabTileValue();
        std::cout << "New Tile Value " << HeldValue << '\n';
      }
      bDragCamera = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
  else if (button == GLFW_MOUSE_BUTTON_LEFT)
  {
    if (action) // Pressed
    {
      bLeftButton = true;
    }
    else // Released
    {
      bLeftButton = false;
      PlaceTileValue(HeldValue);
    }
  }
}


bool GetEscapeKeyState()
{
  return bEscapePressed;
}

glm::vec2 GetLastMousePos()
{
  return glm::vec2(xPosLast, yPosLast);
}
