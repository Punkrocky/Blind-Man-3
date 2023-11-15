/*******************************************************************************************************************//**
 * \file Engine.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once
#include <array>

#include "GlobalDefines.h"
#include "GraphicsSystem.hpp"
#include "GameWindow.hpp"
#include "Logging.hpp"
#include "Walking.hpp"
#include "Entity.hpp"
#include "Chunk.hpp"
#include "Timer.hpp"


static Timer DebugTimer;

class Engine
{
public:
  Engine(GameWindow* window);
  ~Engine();

  void Init();
  void Update(float dt);
  void Shutdown();

  bool IsShuttingDown();
  const glm::ivec2& GetTileCoords(const glm::vec2& worldPos);
  const glm::ivec2& GetChunkCoords(const glm::vec2& worldPos);
  const Entity& GetEntityAtCoords(const glm::ivec2& coords);

private:
  // Input functions
  friend void MoveMouse(double x, double y);
  friend void ScrollMouse(double x, double y);
  friend void SmoothMap();
  friend void GenerateMap();
  friend int GetTileValue();
  friend void SetTileValue(int a);

  GameWindow* PtrGameWindow;      //!< Game Window
  GraphicsSystem* PtrGraphicsSys; //!< Graphics System
  Blind::World* PtrBlindWorld;    //!< World Generator

  ShaderManagerPtr ShManager;  //!< Shader Manager
  TextureManagerPtr TxManager; //!< Texture Manager
  MeshManagerPtr MsManager;    //!< Mesh Manager
  Timer GameTimer;             //!< Timer
  LogFile DebugLog;            //!< Log file for debug information

  EntityPtr EntityArray;                 //!< All Entities
  TransformComponentPtr TransformsArray; //!< All Transform Components for Entities
  GraphicsComponentPtr GraphicsArray;    //!< All Graphics Components for Entites
  std::array<Chunk, CHUNK_PER_WORLD_SQRD>* ChunkArray;

  float dt;            //!< Delta time
  int WorldSizeTiles;  //!< Size of an edge of the world in tiles
  int WorldSizeChunks; //!< Size of the world in chunks
  int ArraySize;       //!< Size of arrays needed to store entities and components
  bool bShuttingDown;  //!< Flag to signal shutdown of the program
};
