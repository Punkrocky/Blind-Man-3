/*******************************************************************************************************************//**
 * \file Engine.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#include "Engine.h"
#include "Random.h"
#include "Input.h"

#include <iostream>

static Random NumberGen(8764);
static Engine* ThisPtr;



void MoveMouse(double x, double y)
{
  ThisPtr->PtrGraphicsSys->MoveCamera(x, y);
}


void ScrollMouse(double x, double y)
{
  ThisPtr->PtrGraphicsSys->ScrollCamera(x, y);
}


void SmoothMap()
{
  ThisPtr->PtrBlindWorld->SmoothMAP();
  for (int i = 0; i < ThisPtr->WorldSize; ++i)
  {
    for (int j = 0; j < ThisPtr->WorldSize; ++j)
    {
      ThisPtr->EntityArray[i * ThisPtr->WorldSize + j].GetGraphicsComponent()->SetColor(ThisPtr->PtrBlindWorld->GetTileColor(i, j, Blind::World::ISLAND_TROPICAL));
    }
  }
}

void GenerateMap()
{
  ThisPtr->PtrBlindWorld->CleanMap();
  ThisPtr->PtrBlindWorld->GenerateIslands();
  ThisPtr->PtrBlindWorld->SmoothMAP();
  ThisPtr->PtrBlindWorld->SmoothMAP();
  for (int i = 0; i < ThisPtr->WorldSize; ++i)
  {
    for (int j = 0; j < ThisPtr->WorldSize; ++j)
    {
      ThisPtr->EntityArray[i * ThisPtr->WorldSize + j].GetGraphicsComponent()->SetColor(ThisPtr->PtrBlindWorld->GetTileColor(i, j, Blind::World::ISLAND_TROPICAL));
    }
  }
}


int GetTileValue()
{
  glm::ivec2 wPos = ThisPtr->GetTileCoords(ThisPtr->PtrGraphicsSys->ViewToWorldTransform(glm::vec4(GetLastMousePos(), 0.0f, 1.0f)));
  return ThisPtr->PtrBlindWorld->GetValue(wPos.y, wPos.x);
}


void SetTileValue(int a)
{
  glm::ivec2 wPos = ThisPtr->GetTileCoords(ThisPtr->PtrGraphicsSys->ViewToWorldTransform(glm::vec4(GetLastMousePos(), 0.0f, 1.0f)));
  ThisPtr->PtrBlindWorld->SetValue(wPos.y, wPos.x, a);
}



Engine::Engine(GameWindow* window) : PtrGameWindow(window), ArraySize(120 * 120), WorldSize(120), bShuttingDown(false)
{
  PtrBlindWorld = new Blind::World(WorldSize, 1);
  PtrBlindWorld->GenerateIslands();
  PtrBlindWorld->SmoothMAP();
  PtrBlindWorld->SmoothMAP();
  PtrBlindWorld->SaveMapImage("Test", WorldSize);

  PtrGraphicsSys = new GraphicsSystem(this);

  ThisPtr = this;
  DragCamera = &MoveMouse;
  ZoomCamera = &ScrollMouse;
  Key_S = &SmoothMap;
  Key_G = &GenerateMap;
  GrabTileValue = &GetTileValue;
  PlaceTileValue = &SetTileValue;

  // Managers
  ShManager = new ShaderManager;
  TxManager = new TextureManager;
  MsManager = new MeshManager;

  // Temp variables to make entities
  glm::vec2 TempPosition(DEFAULT_SCALE);
  glm::vec3 TempColor(1.0f);

  // Allocate memory for all the entities and thier components
  EntityArray = static_cast<EntityPtr>(malloc(sizeof(Entity) * ArraySize));
  TransformsArray = static_cast<TransformComponentPtr>(malloc(sizeof(TransformComponent) * ArraySize));
  GraphicsArray = static_cast<GraphicsComponentPtr>(malloc(sizeof(GraphicsComponent) * ArraySize));

  // Apply colors to the tiles based on their generation
  for (int i = 0; i < WorldSize; ++i)
  {
    for (int j = 0; j < WorldSize; ++j)
    {
      TempColor = PtrBlindWorld->GetTileColor(i, j, Blind::World::PalletType::ISLAND_TROPICAL);
      
      int Index = i * WorldSize + j;
      TransformsArray[Index] = TempPosition;
      GraphicsArray[Index] = TempColor;

      // Create a new Tile in the world
      EntityArray[Index].GraphicsComp = &GraphicsArray[Index];
      EntityArray[Index].InitGraphicsComponent(ShManager->GetShaderPtr(SType::Basic_s), TxManager->GetTexturePtr(TType::White_t), MsManager->GetMeshObject(MType::Square_m));
      EntityArray[Index].TransformComp = &TransformsArray[Index];
      TempPosition.x += DEFAULT_SCALE * 2;
    }

    // Reset the tiles to make the next row
    TempPosition.x = DEFAULT_SCALE;
    TempPosition.y += DEFAULT_SCALE * 2;
  }
  std::cout << "Engine Constructed\n";
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
  int FrameCount = 0;
  float FrameTime = 0.0f;
  while (!bShuttingDown)
  {
    dt = GameTimer.EndFrame();
    FrameTime += dt;
    ++FrameCount;
    //if (dt < 1.0f / 60.0f) // if less than 1 frame has passed don't update anything
    //{
    //  continue;
    //}
    std::cout << "      Total " << GameTimer << '\n';
    GameTimer.StartFrame();

    PtrGraphicsSys->Update(dt, EntityArray, ArraySize);

    if (glfwWindowShouldClose(PtrGameWindow->GetPtrGameWindow()) || GetEscapeKeyState() || FrameCount > 6000)
    {
      bShuttingDown = true;
    }
  }
  float AvgFT = FrameTime / FrameCount;
  std::cout << "Average Frametime: " << AvgFT << '\n';
  std::cout << "Average Framerate: " << 1.0f / AvgFT << '\n';
}


void Engine::Shutdown()
{
  //TODO [3]: Make a system manager.
  //TODO [4]: Move the DestroySystem function to a system manager.
  // Free allocated managers
  delete MsManager;
  delete TxManager;
  delete ShManager;
  // Destroy Each system we created
  DestroySystem(PtrGraphicsSys);

  //TODO [3]: Make an entity manager.
  //TODO [4]: Move the DestroyEntity function to an entity manager.
  // Destroy all the entities created
  //for (Entity obj : EntitiesList)
  {
    //delete &obj;
  }
}


bool Engine::IsShuttingDown()
{
  return bShuttingDown;
}


const glm::ivec2& Engine::GetTileCoords(const glm::vec2& worldPos)
{
  // Divide by the half size to determine which tile we are in the bounds of
  glm::ivec2 TileCoords(worldPos / (DEFAULT_SCALE * 2));

  // Clamp X corods to the border of the world
  if (TileCoords.x >= WorldSize)
  {
    TileCoords.x = WorldSize - 1;
  }
  else if (TileCoords.x < 0)
  {
    TileCoords.x = 0;
  }

  // Clamp the Y coords to the border of the world
  if (TileCoords.y >= WorldSize)
  {
    TileCoords.y = WorldSize - 1;
  }
  else if (TileCoords.y < 0)
  {
    TileCoords.y = 0;
  }
  
  return TileCoords;
}


const Entity& Engine::GetEntityAtCoords(const glm::ivec2& coords)
{
  return EntityArray[coords.y * WorldSize + coords.x];
}
