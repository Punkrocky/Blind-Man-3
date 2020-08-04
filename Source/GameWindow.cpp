/*******************************************************************************************************************//**
 * \file GameWindow.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief
 **********************************************************************************************************************/
#include <iostream>

#include "GameWindow.h"


/****************************| STATIC MEMBERS |*******************************/
GLFWwindow* GameWindow::PtrGameWindow = nullptr;

GameWindow::GameWindow()
{
  // Have glfw initialize itself
  if (!glfwInit())
  {
    std::cerr << "Failed to init GLFW";

    // If glfw does not init properly we want to return a null pointer.
    // TODO: Test this
    delete this;
    return;
  }

  // Tell GLFW which version and prfile of OpenGL we plan on using. We are using version 4.3 and only the core profile.
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Use glfw to make our window.
  PtrGameWindow = glfwCreateWindow(1200, 720, "Blind Man 3", NULL, NULL);
  // Check that the creation process did not error.
  if (PtrGameWindow == NULL)
  {
    std::cerr << "Failed to create window";
    glfwTerminate();

    // If we can not create a window we want to return a null pointer.
    delete this;
    return;
  }

  // Make the new window the context for which future changes will refer to.
  glfwMakeContextCurrent(PtrGameWindow);
}

GameWindow::~GameWindow()
{
  glfwDestroyWindow(PtrGameWindow);
}

GLFWwindow* GameWindow::GetPtrGameWindow()
{
  return PtrGameWindow;
}
