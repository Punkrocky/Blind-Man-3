/*******************************************************************************************************************//**
 * \file GameWindow.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once


#include "OpenGLIncludes.h"

class GameWindow
{
public:
  GameWindow();
  ~GameWindow();

  static GLFWwindow* GetPtrGameWindow();

private:
  static GLFWwindow* PtrGameWindow;
};
