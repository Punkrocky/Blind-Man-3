/*******************************************************************************************************************//**
 * \file Engine.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#include <iostream>

#include "Engine.hpp"
#include "Random.hpp"
#include "Input.hpp"

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
  std::cout << "Smooting Map...\n";
  ThisPtr->PtrBlindWorld->SmoothMAP();
  std::cout << "  Recolor tiles";
  DebugTimer.StartFrame();
  for (int i = 0; i < ThisPtr->WorldSizeTiles; ++i)
  {
    for (int j = 0; j < ThisPtr->WorldSizeTiles; ++j)
    {
      ThisPtr->EntityArray[i * ThisPtr->WorldSizeTiles + j].GetGraphicsComponent()->SetColor(ThisPtr->PtrBlindWorld->GetTileColor(i, j, Blind::World::ISLAND_TROPICAL));
    }
  }
  DebugTimer.EndFrame();
  std::cout << "  " << DebugTimer;

  std::cout << "  Remake chunks";
  DebugTimer.StartFrame();
  for (int i = 0; i < CHUNK_PER_WORLD_SQRD; ++i)
  {
    ThisPtr->ChunkArray[0][i].InitChunkData();
  }
  DebugTimer.EndFrame();
  std::cout << "  " << DebugTimer;
  std::cout << "Finished Smooting\n";
}

void GenerateMap()
{
  std::cout << "Generating Map...";
  DebugTimer.StartFrame();
  ThisPtr->PtrBlindWorld->CleanMap();
  ThisPtr->PtrBlindWorld->GenerateIslands();
  ThisPtr->PtrBlindWorld->SmoothMAP();
  ThisPtr->PtrBlindWorld->SmoothMAP();
  DebugTimer.EndFrame();
  std::cout << "  " << DebugTimer;

  std::cout << "  Recolor tiles";
  DebugTimer.StartFrame();
  for (int i = 0; i < ThisPtr->WorldSizeTiles; ++i)
  {
    for (int j = 0; j < ThisPtr->WorldSizeTiles; ++j)
    {
      ThisPtr->EntityArray[i * ThisPtr->WorldSizeTiles + j].GetGraphicsComponent()->SetColor(ThisPtr->PtrBlindWorld->GetTileColor(i, j, Blind::World::ISLAND_TROPICAL));
    }
  }
  DebugTimer.EndFrame();
  std::cout << "  " << DebugTimer;

  std::cout << "  Remake chunks";
  DebugTimer.StartFrame();
  for (int i = 0; i < CHUNK_PER_WORLD_SQRD; ++i)
  {
    ThisPtr->ChunkArray[0][i].InitChunkData();
  }
  DebugTimer.EndFrame();
  std::cout << "  " << DebugTimer;

  std::cout << "Finished generating\n";
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


Engine::Engine(GameWindow* window) : PtrGameWindow(window), ArraySize(TILES_PER_WORLD_SQRD), WorldSizeTiles(TILES_PER_WORLD), WorldSizeChunks(CHUNK_PER_WORLD), bShuttingDown(false), DebugLog("Debug_Log")
{
  PtrBlindWorld = new Blind::World(WorldSizeTiles, 1);
  PtrBlindWorld->GenerateIslands();
  PtrBlindWorld->SmoothMAP();
  PtrBlindWorld->SmoothMAP();
  PtrBlindWorld->SaveMapImage("Test", WorldSizeTiles);

  PtrGraphicsSys = new GraphicsSystem(this);

  ThisPtr = this;
  DragCamera = &MoveMouse;
  ZoomCamera = &ScrollMouse;
  Key_S = &SmoothMap;
  Key_G = &GenerateMap;
  GrabTileValue = &GetTileValue;
  PlaceTileValue = &SetTileValue;

  DebugLog << "World Size: " << std::to_string(TILES_PER_WORLD) << '\n';
#ifdef _DEBUG
  DebugLog << "Compile mode: Debug\n";
#else
  DebugLog << "Compile mode: Release\n";
#endif

  // Managers
  ShManager = new ShaderManager;
  TxManager = new TextureManager;
  MsManager = new MeshManager;
  Random colorsRands(16346541);
  // Temp variables to make entities
  glm::vec2 TempPosition(TILE_HALF_SIZE);
  glm::vec3 TempColor(1.0f);

  // Allocate memory for all the entities and thier components
  EntityArray = new (std::nothrow) Entity[ArraySize];
  TransformsArray = static_cast<TransformComponentPtr>(malloc(sizeof(TransformComponent) * ArraySize));
  GraphicsArray = static_cast<GraphicsComponentPtr>(malloc(sizeof(GraphicsComponent) * ArraySize));
  ChunkArray = new (std::nothrow) std::array<Chunk, CHUNK_PER_WORLD_SQRD>;


  // Apply colors to the tiles based on their generation
  for (int i = 0; i < WorldSizeTiles; ++i)   // Y-axis
  {
    for (int j = 0; j < WorldSizeTiles; ++j) // X-axis
    {
      TempColor = PtrBlindWorld->GetTileColor(i, j, Blind::World::PalletType::ISLAND_TROPICAL);
      
      int Index = i * WorldSizeTiles + j;
      TransformsArray[Index] = TempPosition;
      GraphicsArray[Index] = TempColor;

      // Create a new Tile in the world
      EntityArray[Index].GraphicsComp = &GraphicsArray[Index];
      EntityArray[Index].InitGraphicsComponent(ShManager->GetShaderPtr(SType::Basic_s), TxManager->GetTexturePtr(TType::White_t), MsManager->GetMeshObject(MType::Square_m));
      EntityArray[Index].TransformComp = &TransformsArray[Index];

      int GC_Index = (i / TILES_PER_CHUNK) * WorldSizeChunks + (j / TILES_PER_CHUNK);
      int IC_Index = (i % TILES_PER_CHUNK) * TILES_PER_CHUNK + (j % TILES_PER_CHUNK);
      (*ChunkArray)[GC_Index].ChunkEntities[IC_Index] = &EntityArray[Index];

      TempPosition.x += TILE_HALF_SIZE * 2;
    }

    // Reset the tiles to make the next row
    TempPosition.x = TILE_HALF_SIZE;
    TempPosition.y += TILE_HALF_SIZE * 2;
  }

  for (int i = 0; i < ChunkArray->size(); ++i)
  {
    ChunkArray[0][i].InitChunkData();
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
  const int MaxFrames = 60000;
  int FrameCount = 0;
  float FrameTime = 0.0f;

  // Game loop
  while (!bShuttingDown)
  {
    dt = GameTimer.EndFrame();
    FrameTime += dt;
    ++FrameCount;
    //if (dt < 1.0f / 60.0f) // if less than 1 frame has passed don't update anything
    //{
    //  continue;
    //}
    //std::cout << "      Total " << GameTimer << '\n';
    GameTimer.StartFrame();

    PtrGraphicsSys->Update(dt, ChunkArray, ChunkArray->size());

    // Test Framerates
    if (glfwWindowShouldClose(PtrGameWindow->GetPtrGameWindow()) || GetEscapeKeyState() || FrameCount >= MaxFrames)
    {
      DebugLog << "Duration: " << FrameCount << " frames\n";
      bShuttingDown = true;
    }
  }

  float AvgFT = FrameTime / FrameCount;
  DebugLog << "Average Frametime: " << std::to_string(AvgFT) << '\n';
  DebugLog << "Average Framerate: " << std::to_string(1.0f / AvgFT) << '\n';
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
  glm::ivec2 TileCoords(worldPos / (TILE_HALF_SIZE * 2));

  // Clamp X corods to the border of the world
  if (TileCoords.x >= WorldSizeTiles)
  {
    TileCoords.x = WorldSizeTiles - 1;
  }
  else if (TileCoords.x < 0)
  {
    TileCoords.x = 0;
  }

  // Clamp the Y coords to the border of the world
  if (TileCoords.y >= WorldSizeTiles)
  {
    TileCoords.y = WorldSizeTiles - 1;
  }
  else if (TileCoords.y < 0)
  {
    TileCoords.y = 0;
  }
  
  return TileCoords;
}

const glm::ivec2& Engine::GetChunkCoords(const glm::vec2& worldPos)
{
  // TODO: insert return statement here
  glm::ivec2 ChunkCoords(worldPos / static_cast<float>(TILES_PER_CHUNK));

  if (ChunkCoords.x >= WorldSizeChunks)
  {
    ChunkCoords.x = WorldSizeChunks - 1;
  }
  else if (ChunkCoords.x < 0)
  {
    ChunkCoords.x = 0;
  }

  if (ChunkCoords.y >= WorldSizeChunks)
  {
    ChunkCoords.y = WorldSizeChunks - 1;
  }
  else if (ChunkCoords.y < 0)
  {
    ChunkCoords.y = 0;
  }

  return ChunkCoords;
}


const Entity& Engine::GetEntityAtCoords(const glm::ivec2& coords)
{
  return EntityArray[coords.y * WorldSizeTiles + coords.x];
}
