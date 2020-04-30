/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GameWindow.h
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
