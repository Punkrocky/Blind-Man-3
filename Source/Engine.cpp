/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: Engine.cpp
 **********************************************************************************************************************/
#include "Engine.h"


Engine::Engine(GameWindow& window) : RefWindow(window)
{
  bShuttingDown = false;
}

Engine::~Engine()
{
  Shutdown();
}

void Engine::Init()
{
  GraphSys.Init();
}

void Engine::Update()
{
  while (!bShuttingDown)
  {
    GraphSys.Update();
  }
}

void Engine::Shutdown()
{
  GraphSys.Shutdown();
}

bool Engine::IsShuttingDown()
{
  return bShuttingDown;
}
