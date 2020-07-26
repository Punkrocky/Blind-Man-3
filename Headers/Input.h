/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 6-18-2020
 * File: Input.h
 **********************************************************************************************************************/
#pragma once

#include "OpenGLIncludes.h"
#include "GraphicsSystem.h"

#include <iostream>

enum CameraDirection
{
  UP = 0,
  DOWN,
  LEFT,
  RIGHT,
  FORWARD,
  BACK,
  ROTATE_CCW,
  ROTATE_CW
};

void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch (key)
  {
  case GLFW_KEY_W:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(UP);
      //std::cout << "W Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_S:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(DOWN);
      //std::cout << "S Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_A:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(LEFT);
      //std::cout << "A Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_D:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(RIGHT);
      //std::cout << "D Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_R:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(FORWARD);
      //std::cout << "R Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_F:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(BACK);
      //std::cout << "F Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_Q:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(ROTATE_CCW);
      //std::cout << "Q Pressed!\n";
    }
  }
  break;
  case GLFW_KEY_E:
  {
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
      GraphicsSystem::MoveCamera(ROTATE_CW);
      //std::cout << "E Pressed!\n";
    }
  }
  break;
  default:
    break;
  }
}
