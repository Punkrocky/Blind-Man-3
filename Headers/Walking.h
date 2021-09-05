/*******************************************************************************************************************//**
 * \file Walking.h
 * \author Matthew LaDouceur
 * \date 6-1-20118
 * \brief Header for the map generation
 **********************************************************************************************************************/
#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <chrono>



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
    DIRECTION RandomInt(int max, int min = 0);
    void CenterPosition();
    void RandomPosition();

    std::mt19937 Generator;
    int Position; // Current position on the map
    int Seed; // Starting condition for the RNG
    int Size; // Size of the map this mole is attached to
  };

  class World
  {
  public:
    World(int size, int moleCount);
    ~World();

    //Getters and Setters
    int GetValue(int x, int y);
    int GetValue(int position);
    void SetValue(int x, int y, int value);
    int GetLargestValue();

    //Map generating functions
    void MAPGenerateGenOrder();
    void MAPGenerateIslandMap();

    //Map manipulation functions
    void CleanMap();
    void SmoothMAP();
    void SaveMap(std::string name);
    void SaveMapImage(std::string name, int size);
    void ReadMap(std::string name);

  private:
    std::string filename;   // Name for the image file
    std::ofstream TileFile; // Handle for creation an output image

    std::vector<std::vector<int>> TileMap; // Intager representation of the generated world
    std::vector<Mole> MoleArray;           // Stores all the Moles used to generate this map
    int LargestValue;                      // Global maximum on this map
    int Size;                              // Size of the map to generate
  };
}