/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: Engine.cpp
 **********************************************************************************************************************/
#include "Engine.h"
#include "Random.h"

#include <iostream>

static Random NumberGen(1000);

Engine::Engine(GameWindow* window) : PtrGameWindow(window)
{
  bShuttingDown = false;
  PtrGraphicsSys = new GraphicsSystem;

  glm::vec2 TempPosition((-80.0f + DEFAULT_SCALE), (-80 + DEFAULT_SCALE));
  glm::vec3 TempColor(1.0f);

  int SizeI = ((2000 / DEFAULT_SCALE));
  int SizeJ = ((2000 / DEFAULT_SCALE));

  for (int i = 0; i < SizeI; ++i)
  {
    //TempColor.g = 1.0f - (TempPosition.y / 720);
    for (int j = 0; j < SizeJ; ++j)
    {
      //TempColor.r = 1.0f - (TempPosition.x / 1200);
      TempColor.r = NumberGen.GenerateRandomFloat(1.0f);
      TempColor.g = NumberGen.GenerateRandomFloat(1.0f);
      TempColor.b = NumberGen.GenerateRandomFloat(1.0f);
      PtrEntities.emplace_back(new Entity(TempPosition, TempColor));
      TempPosition.x += (DEFAULT_SCALE*2);
    }
    TempPosition.x = (-80.0f + DEFAULT_SCALE);
    TempPosition.y += (DEFAULT_SCALE*2);
  }
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

    PtrGraphicsSys->Update(dt, PtrEntities);

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
