/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: Engine.h
 **********************************************************************************************************************/
#pragma once

#include "GraphicsSystem.h"
#include "GraphicsSystem.h"
#include "GameWindow.h"
#include "Entity.h"
#include "Timer.h"

#include <vector>

class Engine
{
public:
  Engine(GameWindow* window);
  ~Engine();

  void Init();
  void Update(float dt);
  void Shutdown();

  bool IsShuttingDown();

private:
  GameWindow* PtrGameWindow;
  GraphicsSystem* PtrGraphicsSys;
  std::vector<EntityPtr> PtrEntities;
  Timer GameTimer;
  bool bShuttingDown;
  float dt;
};
