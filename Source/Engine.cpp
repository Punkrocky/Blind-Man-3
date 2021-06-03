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

glm::vec3 operator/(const glm::vec3& l, float r)
{
  glm::vec3 temp(l);
  temp.x /= r;
  temp.y /= r;
  temp.z /= r;
  return temp;
}

//glm::vec3 operator*(const glm::vec3& l, float r)
//{
//  glm::vec3 temp(l);
//  temp.x *= r;
//  temp.y *= r;
//  temp.z *= r;
//  return temp;
//}

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
      /*TempColor.r = NumberGen.GenerateRandomFloat(1.0f, 0.70f);
      TempColor.g = NumberGen.GenerateRandomFloat(1.0f, 0.70f);
      TempColor.b = NumberGen.GenerateRandomFloat(1.0f, 0.70f);*/

      // Don't alter the color of the texture at all
      TempColor.r = 1.0f;
      TempColor.g = 1.0f;
      TempColor.b = 1.0f;

      // Create a new Tile in the world
      TransformComponent trans(TempPosition, DEFAULT_SCALE);
      GraphicsComponent graph(Texture::TextureType::White_t);
      float f = ((static_cast<float>(i) * j) / (static_cast<float>(SizeI) * SizeJ));
      graph.SetGraphicsComponentColor(TempColor * glm::vec3(f));
      EntitiesList[i * SizeJ + j] = new Entity(trans, graph);

      TempPosition.x += DEFAULT_SCALE * 2;
    }

    // Reset the tiles to make the next row
    TempPosition.x = 0;
    TempPosition.y += DEFAULT_SCALE * 2;
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
