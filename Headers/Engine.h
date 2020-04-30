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

class Engine
{
public:
  Engine(GameWindow& window);
  ~Engine();

  void Init();
  void Update();
  void Shutdown();

  bool IsShuttingDown();

private:
  GameWindow& RefWindow;
  GraphicsSystem GraphSys;
  bool bShuttingDown;
};
