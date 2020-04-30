/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: main.cpp
 **********************************************************************************************************************/
//#include <stdio.h>
#include <iostream>

#include "GraphicsSystem.h"
#include "GameWindow.h"
#include "Engine.h"

int main(void)
{
  // Create our window on the heap so we can guarantee its lifetime
  GameWindow* PtrWindow = new GameWindow();
  std::cout << "Hello World!\n";

  // Check that we successfully created a window.
  if (PtrWindow)
  {
    Engine GameEngine(*PtrWindow);
    GameEngine.Init();
  
    GameEngine.Update();

    delete PtrWindow;
  }
  return 0;
}