/*******************************************************************************************************************//**
 * \file Engine.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#include "Engine.h"
#include "Random.h"

#include <iostream>

static Random NumberGen(1000);


Engine::Engine(GameWindow* window) : PtrGameWindow(window)
{
  bShuttingDown = false;
  PtrGraphicsSys = new GraphicsSystem();

  // Temp variables to make entities
  glm::vec2 TempPosition(0);
  glm::vec3 TempColor(1.0f);

  // Dimentions of the Tiled world
  const int SizeI = 32;
  const int SizeJ = 32;

  EntitiesList.resize(SizeI * SizeJ);

  for (int i = 0; i < SizeI; ++i)
  {
    for (int j = 0; j < SizeJ; ++j)
    {
      // Generate a random color to multiply with the texture of the entity
      TempColor.r = NumberGen.GenerateRandomFloat(1.0f, 0.75f);
      TempColor.g = NumberGen.GenerateRandomFloat(1.0f, 0.75f);
      TempColor.b = NumberGen.GenerateRandomFloat(1.0f, 0.75f);

      // Create a new Tile in the world
      TransformComponent trans(TempPosition, glm::vec2(DEFAULT_SCALE, DEFAULT_SCALE));
      GraphicsComponent graph(Texture::TextureType::Farm_t);
      graph.SetGraphicsComponentColor(TempColor);
      EntitiesList[i * SizeJ + j] = new Entity(trans, graph);

      // Layer Tiles with a slope of 1/2 for a nice isometric view
      TempPosition.x -= DEFAULT_SCALE;
      TempPosition.y -= (DEFAULT_SCALE/2);
    }

    // Reset the tiles to make the next row
    TempPosition.x = DEFAULT_SCALE * i;
    TempPosition.y = (-DEFAULT_SCALE / 2) * i;
  }
}


Engine::~Engine()
{
  Shutdown();
}


void Engine::Init()
{
  // Empty because evrything can be done in the construtor
}


void Engine::Update(float dt)
{
  int count = 0;
  while (!bShuttingDown)
  {
    dt = GameTimer.EndFrame();
    if (dt < 1.0f / 60.0f) // if less than 1 frame has passed don't update anything
    {
      continue;
    }
    GameTimer.StartFrame();


    PtrGraphicsSys->Update(dt, EntitiesList);

    if (glfwWindowShouldClose(PtrGameWindow->GetPtrGameWindow()))
    {
      bShuttingDown = true;
    }
  }
}


void Engine::Shutdown()
{
  //TODO [3]: Make a system manager.
  //TODO [4]: Move the DestroySystem function to a system manager.
  // Destroy Each system we created
  DestroySystem(PtrGraphicsSys);

  //TODO [3]: Make an entity manager.
  //TODO [4]: Move the DestroyEntity function to an entity manager.
  // Destroy all the entities created
  for (EntityPtr obj : EntitiesList)
  {
    DestroyEntity(obj);
  }
}


bool Engine::IsShuttingDown()
{
  return bShuttingDown;
}
