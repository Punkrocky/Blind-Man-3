/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: Engine.cpp
 **********************************************************************************************************************/
#include "Engine.h"


Engine::Engine(GameWindow* window) : PtrGameWindow(window)
{
  bShuttingDown = false;
  PtrGraphicsSys = new GraphicsSystem;
}

Engine::~Engine()
{
  Shutdown();
}

void Engine::Init()
{
  //TODO: Remove?
  //PtrGraphicsSys->Init();
}

void Engine::Update()
{
  while (!bShuttingDown)
  {
    PtrGraphicsSys->Update();

    if (glfwWindowShouldClose(PtrGameWindow->GetPtrGameWindow()))
    {
      bShuttingDown = true;
    }
  }
}

void Engine::Shutdown()
{
  //PtrGraphicsSys->Shutdown();
  DestroySystem(PtrGraphicsSys);
}

bool Engine::IsShuttingDown()
{
  return bShuttingDown;
}
