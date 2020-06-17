/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: Engine.cpp
 **********************************************************************************************************************/
#include "Engine.h"

#include <iostream>

Engine::Engine(GameWindow* window) : PtrGameWindow(window)
{
  bShuttingDown = false;
  PtrGraphicsSys = new GraphicsSystem;
  entity = new Entity;
  //entity->GetTransformComponent()->SetPosition(1000.0f, -500.0f);
}

Engine::~Engine()
{
  Shutdown();
}

void Engine::Init()
{
}

void Engine::Update(float dt)
{
  while (!bShuttingDown)
  {
    dt = GameTimer.EndFrame();
    if (dt < 1.0f / 60.0f) // if less than 1 frame has passed don't update anything
    {
      continue;
    }
    //std::cout << dt << "\n";
    GameTimer.StartFrame();

    PtrGraphicsSys->Update(dt, entity);

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
