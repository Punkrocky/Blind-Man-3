/*******************************************************************************************************************//**
 * \file Walking.h
 * \author Matthew LaDouceur
 * \date 6-1-2018
 * \brief Header for the map generation
 **********************************************************************************************************************/
#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <chrono>
#include <utility>

#include "OpenGLIncludes.hpp"
#include "Random.hpp"

namespace Blind
{
  class World;

  enum DIRECTION
  {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
  };

  class Mole
  {
  public:
    Mole() = delete;
    Mole(int seed, int size);

    //Assignment Operator
    Mole& operator=(const Mole& mole);
    int RandomDirection();

  private:
    // Helper functions
    DIRECTION RandomDirection(int max, int min = 0);
    void CenterPosition();
    void RandomPosition();

    //std::mt19937 Generator;
    Random RNG;   // Creates random numbers
    int Position; // Current position on the map
    int Seed;     // Starting condition for the RNG
    int Size;     // Size of the map this mole is attached to
  };


  enum TileTypes
  {
    NONE = 0,
    WALL,
    ROOM_SMALL,
    ROOM_MEDIUM,
    ROOM_LARGE
  };

  enum TileTypesIsland
  {
    DEEP_OCEAN = 0,
    SHALLOW_OCEAN = 1,
    BEACH = 6,
    GRASS = 9,
    MOUNTAIN = 21,
    MOUNTAIN_PEAKS = 40
  };

  class Island
  {
  public:
    Island();
    Island(int id, glm::vec3 color);
    ~Island();

    glm::vec3 GetColor() const;
    int GetBoundsAtIndex(Blind::DIRECTION index) const;

    //void ExploreIsland(int position); // Depth-first search to find the extent of an island

  private:
    int ID;                 // Number value to identify this island
    int Size;               // Total number of tiles in this island
    glm::vec3 Color;        // The color tiles of this island are rendered as
    int Bounds[4];          // 0=TOP 1=BOTTOM 2=LEFT 3=RIGHT use enum DIRECTION as index
    std::vector<int> Tiles; // An array of all the tiles apart of this island

    friend class World;
  };

  class World
  {
  public:
    World() = delete;
    World(int size, int moleCount);
    ~World();

    enum PalletType
    {
      YELLOW_PURPLE = 0,
      ISLAND_TROPICAL,
      VISITED,
      GRAY_SCALE
    };

    //Getters and Setters
    int GetValue(int x, int y);
    int GetValue(int position);
    void SetValue(int x, int y, int value);
    int GetLargestValue();
    glm::vec3 GetTileColor(int x, int y, PalletType type);
    std::vector<Blind::Island>& GetIslandArray();
    std::vector<std::vector<std::array<glm::vec4, 9>>>& GetGradientMap();

    //Map generating functions
    void GenerateInOrder();
    void GenerateIslands();
    void GenerateDungeon();
    void GenerateDiamondSquare();
    void GenerateCirclePlot();

    void Noise();
    void CountIslands();

    //Map manipulation functions
    void CleanMap();
    void SmoothMAP();
    void MakeGradientMAP();
    void ErodeAndDeposit();

    void SaveMap(std::string name);
    void SaveMapImage(std::string name, int size, Blind::World::PalletType type, unsigned ColorShift);
    void ReadMap(std::string name);

  private:
    float Fade(float t);
    float Lerp(float t, float x, float y);

    std::string filename;   // Name for the image file
    std::ofstream TileFile; // Handle for creation an output image
    Random WorldRNG;        // Base for the noise function

    std::vector<std::vector<float>> TileMap;      // Number representation of the generated world
    std::vector<std::vector<int>> VisitMap;       // Visited representation of the generated world
    std::vector<std::vector<std::array<glm::vec4, 9>>> GradientMap;  // Vector representation of the slope of the generated world
    std::vector<int> Peaks;
    std::vector<std::vector<glm::vec3>> ColorMap; // Color representation of the generated world

    std::vector<Island> IslandArray; // Stores all unique islands generated on this map
    std::vector<Mole> MoleArray;     // Stores all the Moles used to generate this map

    int LargestValue;  // Global maximum on this map
    int SmallestValue; // Global maximum on this map
    int Size;          // Size of the map to generate
    int IslandCount;   // Number of islands generated on the map
  };
}
