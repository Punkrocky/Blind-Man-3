/*******************************************************************************************************************//**
 * \file main.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief 
 **********************************************************************************************************************/
#include <iostream>

#include "GraphicsSystem.h"
#include "GameWindow.h"
#include "Engine.h"

int main(void)
{
  float dt = 0.0f;
  // Create our window on the heap so we can guarantee its lifetime
  GameWindow* PtrWindow = new GameWindow();

  // Check that we successfully created a window
  if (PtrWindow)
  {
    Engine GameEngine(PtrWindow);
    GameEngine.Init();
  
    GameEngine.Update(dt);

    delete PtrWindow;
  }
  return 0;
}