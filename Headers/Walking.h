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
#include <string>
#include <chrono>

#include "Random.h"
#include "OpenGLIncludes.h"

namespace Blind
{
  class Mole
  {
  public:
    Mole() = delete;
    Mole(int seed, int size);

    //Assignment Operator
    Mole& operator=(const Mole& mole);
    int RandomDirection();

  private:
    enum DIRECTION
    {
      UP = 0,
      DOWN,
      LEFT,
      RIGHT
    };

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

  class World
  {
  public:
    World() = delete;
    World(int size, int moleCount);
    ~World();

    enum PalletType
    {
      YELLOW_PURPLE = 0,
      ISLAND_TROPICAL
    };

    //Getters and Setters
    int GetValue(int y, int x);
    int GetValue(int position);
    void SetValue(int y, int x, int value);
    int GetLargestValue();
    glm::vec3 GetTileColor(int y, int x, PalletType type);

    //Map generating functions
    void GenerateInOrder();
    void GenerateIslands();
    void GenerateDungeon();

    void Noise();

    //Map manipulation functions
    void CleanMap();
    void SmoothMAP();
    void SaveMap(std::string name);
    void SaveMapImage(std::string name, int size);
    void ReadMap(std::string name);

  private:
    float Fade(float t);
    float Lerp(float t, float x, float y);

    std::string filename;   // Name for the image file
    std::ofstream TileFile; // Handle for creation an output image
    Random WorldRNG;        // Base for the noise function

    std::vector<std::vector<float>> TileMap; // Intager representation of the generated world
    std::vector<Mole> MoleArray;             // Stores all the Moles used to generate this map
    int LargestValue;                        // Global maximum on this map
    int Size;                                // Size of the map to generate
  };
}
