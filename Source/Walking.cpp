/*******************************************************************************************************************//**
 * \file Walking.cpp
 * \author Matthew LaDouceur
 * \date 6-8-2021
 * \brief Source for the map generation
 **********************************************************************************************************************/

#include "Walking.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "GlobalDefines.hpp"
#include <stdlib.h>

#include <STB/stb_image.h>
#ifndef STBI_MSC_SECURE_CRT
#define STBI_MSC_SECURE_CRT
#endif // !STBI_MSC_SECURE_CRT
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif // !STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image_write.h>

#include <deque>

#define MAP_POS_INDEX TileMap[Position / Size][Position % Size]

#define CHECK_LEFT(x)  x % Size == 0
#define CHECK_RIGHT(x) x % Size == (Size - 1)
#define CHECK_DOWN(x)  x >= (Size * (Size - 1))
#define CHECK_UP(x)    x < Size

#define MOLE_MOVE_LEFT  --Position
#define MOLE_MOVE_RIGHT ++Position
#define MOLE_MOVE_DOWN  Position += Size
#define MOLE_MOVE_UP    Position -= Size

static Timer DebugWgenTimer;

///*                                Order of Acess
//                                  / 3  4  5 \
//                                  | 2  X  6 |
//                                  \ 1  8  7 /
//*/
//static const int RowOffsets[] =    { -1,  0,  1, 1, 1, 0, -1, -1 }; // Y-axis
//static const int ColumnOffsets[] = { -1, -1, -1, 0, 1, 1,  1,  0 }; // X-axis
///*                                Order of Acess
//                                  / 3  7  1 \
//                                  | 6  X  5 |
//                                  \ 4  8  2 /
//*/
//static const int RowOffsets[] = { 1, -1,  1, -1,  0,  0,  1, -1 }; // Y-axis
//static const int ColumnOffsets[] = { 1,  1, -1, -1,  1, -1,  0,  0 }; // X-axis
/*                                Order of Acess
                                  / 1  2  3 \
                                  | 4  X  5 |
                                  \ 6  7  8 /
*/
static const int RowOffsets[] =    {  1,  1,  1,  0, 0, 0, -1, -1, -1 }; // Y-axis
static const int ColumnOffsets[] = { -1,  0,  1, -1, 0, 1, -1,  0,  1 }; // X-axis

Blind::World::World(int size, int moleCount) : Size(size), WorldRNG(Random(1615134))
{
  MoleArray.reserve(moleCount);
  for (int i = 0; i < moleCount; ++i)
  {
    MoleArray.push_back(Mole(19742, Size));
  }

  TileMap.resize(Size);
  for (int i = 0; i < Size; ++i)
  {
    TileMap[i].resize(Size);
  }

  VisitMap.resize(Size);
  for (int i = 0; i < Size; ++i)
  {
    VisitMap[i].resize(Size);
  }

  stbi_flip_vertically_on_write(true);
}

Blind::World::~World()
{

}

int Blind::World::GetValue(int x, int y)
{
  return this->TileMap[x][y];
}

int Blind::World::GetValue(int position)
{
  int Position = position;
  return this->MAP_POS_INDEX;
}

void Blind::World::SetValue(int x, int y, int value)
{
  this->TileMap[x][y] = value;
}

int Blind::World::GetLargestValue()
{
  std::cout << LargestValue << '\n';
  return LargestValue;
}

glm::vec3 Blind::World::GetTileColor(int x, int y, PalletType type)
{
  //DebugWgenTimer.StartFrame();

  glm::vec3 TempColor(0.0f);
  // Border
  if (x == 0 || y == 0 || x == (Size - 1) || y == (Size - 1))
  {
    return TempColor;
  }
  // Set color for sea level tiles
  if (TileMap[x][y] < SEA_LEVEL)
  {
    TempColor.r = 0.0f;
    TempColor.g = 0.85f;
    TempColor.b = 0.15f;
    return TempColor;
  }

  switch (type)
  {
  case Blind::World::YELLOW_PURPLE:
  {
    // Create a yellow purple gradient from high to low

    // Convert the range of values from [SmallestValue, LargestValue] into the range [-1, 1]
    float Fi = 2 * ((TileMap[x][y] - SmallestValue) / (LargestValue - SmallestValue)) - 1;

    //float Fo = ((((1 * Fi - 5) * Fi + 7) * Fi - 5) * Fi + 3) * Fi;
    //float Fo = ((((3 * Fi - 6) * Fi + 7) * Fi - 6) * Fi + 3) * Fi;
    float Fo = (((3 * Fi * Fi - 10) * Fi * Fi + 17) * Fi) / 10.0f; // An S-curve over the range [-1, 1]

    /*float epsilon = 0.025f;
    if (Fo < 0.0f + epsilon && Fo > 0.0f - epsilon)
    {
      TempColor = glm::vec3(1.0f);
      break;
    }*/

    float R = 0.50f + Fo * (0.4);
    float G = 0.10f + Fo * (0.2);
    float B = 0.30f + Fo * (-0.1);

    TempColor.r = R;
    TempColor.g = G;
    TempColor.b = B;

    
    break;
  }
  case Blind::World::ISLAND_TROPICAL:
  {
    int Fi = TileMap[x][y];
    if (Fi == DEEP_OCEAN)                           // Deep Water
    {
      TempColor.r = 0.01f;
      TempColor.g = 0.42f;
      TempColor.b = 0.71f;
    }
    else if (Fi >= SHALLOW_OCEAN && Fi < BEACH)     // Shallow Water
    {
      TempColor.r = 0.13f;
      TempColor.g = 0.73f;
      TempColor.b = 0.87f;
    }
    else if (Fi >= BEACH && Fi < GRASS)             // Beach
    {
      TempColor.r = 0.92f;
      TempColor.g = 0.80f;
      TempColor.b = 0.50f;
    }
    else if (Fi >= GRASS && Fi < MOUNTAIN)          // Grass
    {
      TempColor.r = 0.05f;
      TempColor.g = 0.74f;
      TempColor.b = 0.13f;
    }
    else if (Fi >= MOUNTAIN && Fi < MOUNTAIN_PEAKS) // Mountain
    {
      TempColor.r = 0.75f;
      TempColor.g = 0.75f;
      TempColor.b = 0.75f;
    }
    else if (Fi >= MOUNTAIN_PEAKS)                  // Mountain Peaks
    {
      TempColor.r = 0.85f;
      TempColor.g = 0.80f;
      TempColor.b = 0.95f;
    }
    break;
  }
  case VISITED:
  {
    TempColor = IslandArray[VisitMap[x][y]].GetColor();
    break;
  }
  case GRAY_SCALE:
  {
    float Fi = TileMap[x][y];

    if (Fi > 0.0f)
    {
      TempColor = glm::vec3(1.0f);
    }
    else if (Fi < 0.0f)
    {
      TempColor = glm::vec3(0.0f);
    }
    else
    {
      TempColor = glm::vec3(0.5f);
    }

    break;
  }
  default:
    break;
  }

  //DebugWgenTimer.EndFrame();
  //std::cout << "    " << DebugWgenTimer;

  return TempColor;
}

std::vector<Blind::Island>& Blind::World::GetIslandArray()
{
  return IslandArray;
}

std::vector<std::vector<std::array<glm::vec4, 9>>>& Blind::World::GetGradientMap()
{
  return GradientMap;
}

void Blind::World::GenerateInOrder()
{
  int floortiles = 0; // Count the number of altered tiles to stop generation
  int attempts = 0;   // Count the total number of generation attempts to prevent livelock

  do
  {
    int Position = 0;
    for (Blind::Mole& m : MoleArray)
    {
      Position = m.RandomDirection();

      // If the tile is unaltered we will update it
      if (MAP_POS_INDEX == 0)
      {
        // Increment the number of altered tiles
        ++floortiles;
        // The value of the tile will match the order it was generated in
        MAP_POS_INDEX = floortiles;
      }

      // Increment the number of generation attempts
      ++attempts;
    }
  } while (floortiles < (Size * Size) / 3 && attempts < 1000000);
  LargestValue = floortiles;
}

void Blind::World::GenerateIslands()
{
  int floortiles = 0; // Count the number of altered tiles to stop generation
  int attempts = 0;   // Count the total number of generation attempts to prevent livelock
  int UniqueTiles = 0;

  do
  {
    int Position = 0;
    for (Blind::Mole& m : MoleArray)
    {
      // Tell the Mole to move
      Position = m.RandomDirection();

      if (MAP_POS_INDEX == 0)
      {
        ++UniqueTiles;
      }

      // Increment the number of tiles that have been altered
      ++floortiles;

      // Increment the number of total generation attempts made
      ++attempts;

      // Increment the value of the tile at the new position
      MAP_POS_INDEX++;

      // Update the largest value on this map
      if (MAP_POS_INDEX > LargestValue)
      {
        LargestValue = MAP_POS_INDEX;
      }
    }
  } while (floortiles < ((Size * Size) * 3) && attempts < 10000000);

  for (int i = 0; i < Size; ++i)
  {
    TileMap[0][i] = 0;
    TileMap[i][0] = 0;
    TileMap[Size - 1][i] = 0;
    TileMap[i][Size - 1] = 0;
  }
}

/// <summary>
/// Generate a series of rooms (small, medium, and large) with corridors connecting them.
/// </summary>
void Blind::World::GenerateDungeon()
{
  // number of small, medium, and large rooms to try generating
  int s = 7, m = 5, l = 3;


}

void Blind::World::GenerateDiamondSquare()
{

}

void Blind::World::GenerateCirclePlot()
{
  int Radius = 256;
  int Position = 0;

  for(int t = 1; Radius > 2; Radius /= 2, ++t)
  {
    for (int i = 0; i < 1000 * t; ++i)
    {
      Position = WorldRNG.GenerateRandomInt((Size - 1) * (Size - 1));
      float Offset = WorldRNG.GenerateRandomFloat(1.0f * t * 2, -1.0f * t * 2);
      int PositionY = Position / Size;
      int PositionX = Position % Size;

      for (int j = -Radius; j < Radius; ++j)
      {
        for (int k = -Radius; k < Radius; ++k)
        {
          // Make sure we are not about to leave the bounds of the map
          if (PositionX + k >= Size)
          {
            continue;
          }
          else if (PositionX + k < 0)
          {
            continue;
          }

          if (PositionY + j < 0)
          {
            continue;
          }
          else if (PositionY + j >= Size)
          {
            continue;
          }

          int DeltaX = ((PositionX + k) - PositionX);
          int DeltaY = ((PositionY + j) - PositionY);

          int DistanceSqrd = DeltaX * DeltaX + DeltaY * DeltaY;

          if (DistanceSqrd < Radius * Radius)
          {
            TileMap[j + PositionY][k + PositionX] += Offset;
            if (TileMap[PositionY + j][PositionX + k] > LargestValue)
            {
              LargestValue = TileMap[PositionY + j][PositionX + k];
            }
            else if (TileMap[PositionY + j][PositionX + k] < SmallestValue)
            {
              SmallestValue = TileMap[PositionY + j][PositionX + k];
            }
          }
        }
      }
    }

    //SmoothMAP();
    char buffer[8];
    _itoa_s(Radius, buffer, _countof(buffer), 10);
    SaveMapImage(buffer, Size, YELLOW_PURPLE, 0);
    //CleanMap();
  }
  std::cout << "Largest: " << LargestValue << " Smallest: " << SmallestValue << std::endl;
}

/// <summary>
/// 
/// </summary>
void Blind::World::Noise()
{
  //glm::vec2 GlobalVec = glm::vec2(WorldRNG.GenerateRandomFloat(1.0f, -1.0f), WorldRNG.GenerateRandomFloat(1.0f, -1.0f));

  std::vector<glm::vec2> VertVector(2 * (Size + 1));
  for (int i = 0; i < (Size + 1); ++i) // Generate the top row of vectors
  {
    VertVector[i] = glm::vec2(WorldRNG.GenerateRandomFloat(1.0f, -1.0f), WorldRNG.GenerateRandomFloat(1.0f, -1.0f));
  }

  for (int i = 0; i < Size; ++i)
  {
    for (int k = 0; k < (Size + 1); ++k) // Generate the next row of vectors
    {
      VertVector[k + (Size + 1)] = glm::vec2(WorldRNG.GenerateRandomFloat(1.0f, -1.0f), WorldRNG.GenerateRandomFloat(1.0f, -1.0f));
    }
    for (int j = 0; j < Size; ++j)
    {
      glm::vec2 InsidePos = glm::vec2(WorldRNG.GenerateRandomFloat(1.0f, -1.0f), WorldRNG.GenerateRandomFloat(1.0f, -1.0f));
      glm::vec2 TL(-1.0f, 1.0f), TR(1.0f, 1.0f), BL(-1.0f, -1.0f), BR(1.0f, -1.0f);
      TL = InsidePos - TL;
      TR = InsidePos - TR;
      BL = InsidePos - BL;
      BR = InsidePos - BR;

      TL = glm::normalize(TL);
      TR = glm::normalize(TR);
      BL = glm::normalize(BL);
      BR = glm::normalize(BR);

      float TLv = glm::dot(VertVector[j], TL);
      float TRv = glm::dot(VertVector[j + 1], TR);
      float BLv = glm::dot(VertVector[j + (Size + 1)], BL);
      float BRv = glm::dot(VertVector[(j + 1) + (Size + 1)], BR);

      float Fx = Fade((InsidePos.x + 1.0f) / 2.0f);
      float Fy = Fade((InsidePos.y + 1.0f) / 2.0f);

      /*Fx = (Fx + 1.0f) / 2.0f;
      Fy = (Fy + 1.0f) / 2.0f;*/

      float result = Lerp(Fx, Lerp(Fy, TLv, BLv), Lerp(Fy, TRv, BRv));
      result = (result + 1.0f) / 2.0f;

      if (result > LargestValue)
      {
        LargestValue = result;
      }
      TileMap[i][j] = result;
    }
    for (int l = 0; l < (Size + 1); ++l) // Copy the second row of values into the first row
    {
      VertVector[l] = VertVector[l + (Size + 1)];
    }
  }
}



void Blind::World::CountIslands()
{
  // Default color for any tile not part of an island
  IslandArray.push_back(Island());
  std::deque<int> Neighbors;

  for (int i = 0; i < Size; ++i) // Rows (Y-axis)
  {
    for (int j = 0; j < Size; ++j) // Columns (X-axis)
    {
      if (TileMap[i][j] >= BEACH && VisitMap[i][j] == 0) // Add any land tile that has not been visited yet
      {
        int Position = i * Size + j;
        int numtiles = 0; // Tracks the number of tiles that belong to the current island
        Neighbors.push_back(Position);
        Island CurrentIsland(++IslandCount, glm::vec3(1.0f));

        while (!Neighbors.empty()) // Preform a search around the unvisited land tile to visit the entire connected island
        {
          int Position = Neighbors.front();
          int PositionI = Position / Size;
          int PositionJ = Position % Size;
          Neighbors.pop_front();

          // Skip any duplicate tiles that were added
          if (VisitMap[PositionI][PositionJ] != 0)
          {
            continue;
          }
          VisitMap[PositionI][PositionJ] = IslandCount; // Mark the current tile as visited
          ++numtiles;

          CurrentIsland.Tiles.push_back(Position);

          if (PositionI > CurrentIsland.Bounds[UP])
          {
            CurrentIsland.Bounds[UP] = PositionI;
          }
          if (PositionI < CurrentIsland.Bounds[DOWN])
          {
            CurrentIsland.Bounds[DOWN] = PositionI;
          }

          if (PositionJ < CurrentIsland.Bounds[LEFT])
          {
            CurrentIsland.Bounds[LEFT] = PositionJ;
          }
          if (PositionJ > CurrentIsland.Bounds[RIGHT])
          {
            CurrentIsland.Bounds[RIGHT] = PositionJ;
          }

          // Look for any neighboring land tiles
          for (int k = 0; k < 9; ++k)
          {
            int NeighborPositionI = PositionI + RowOffsets[k];
            int NeighborPositionJ = PositionJ + ColumnOffsets[k];
            int NeighborPosition = NeighborPositionI * Size + NeighborPositionJ;

            // Check that the current position is not out of bounds
            if (CHECK_LEFT(NeighborPosition))
            {
              continue;
            }
            else if (CHECK_RIGHT(NeighborPosition))
            {
              continue;
            }

            if (CHECK_UP(NeighborPosition))
            {
              continue;
            }
            else if (CHECK_DOWN(NeighborPosition))
            {
              continue;
            }

            // Only add unvisited land tiles
            if (VisitMap[NeighborPositionI][NeighborPositionJ] == 0 && TileMap[NeighborPositionI][NeighborPositionJ] >= BEACH)
            {
              Neighbors.push_back(NeighborPosition);
            }
          }
        }

        // Give a random color to any island with over 150 tiles
        if (numtiles > 150)
        {
          CurrentIsland.Color = glm::vec3(WorldRNG.GenerateRandomFloat(1.0f), WorldRNG.GenerateRandomFloat(1.0f), WorldRNG.GenerateRandomFloat(1.0f));
        }
        CurrentIsland.Size = numtiles;
        IslandArray.push_back(CurrentIsland);
        std::cout << "Island " << IslandCount << " Number of Tiles: " << numtiles
          << " Color: " << IslandArray[IslandCount].GetColor() 
          << "\n\n";
      }
    }
  }

  std::cout << "Island Count: " << IslandCount << '\n';
}

/// <summary>
/// Reset the entire map to 0's
/// </summary>
void Blind::World::CleanMap()
{
  //std::memset(&TileMap[0], 0, Size * Size * sizeof(int));

  for (int i = 0; i < Size; ++i)
  {
    std::memset(&TileMap[i][0], 0, Size * sizeof(float));
    std::memset(&VisitMap[i][0], 0, Size * sizeof(int));
  }
  IslandArray.clear();
  LargestValue = 0;
  SmallestValue = 0;
  IslandCount = 0;
}

void Blind::World::SmoothMAP()
{
  // Move through the entire map
  for (int Position = 0; Position < (Size * Size); ++Position)
  {
    int Temp_position = Position;
    int average = 0;

    --Position; // Move Left
    if (Position > 0)
    {
      average += MAP_POS_INDEX;
    }
    Position = Temp_position; // Reset
    ++Position; // Move Right
    if (Position < Size * Size)
    {
      average += MAP_POS_INDEX;
    }
    Position = Temp_position; // Reset
    Position -= Size; // Move Down
    if (Position > 0)
    {
      average += MAP_POS_INDEX;
    }
    Position = Temp_position; // Reset
    Position += Size; // Move Up
    if (Position < Size * Size)
    {
      average += MAP_POS_INDEX;
    }
    Position = Temp_position; // Reset
    MAP_POS_INDEX = average / 4;
  }

  for (int i = 0; i < Size; ++i)
  {
    TileMap[0][i] = 0;
    TileMap[i][0] = 0;
    TileMap[Size - 1][i] = 0;
    TileMap[i][Size - 1] = 0;
  }
}

void Blind::World::MakeGradientMAP()
{
  GradientMap.resize(Size);
  for (int i = 0; i < Size; ++i)
  {
    GradientMap[i].resize(Size);
  }

  // Tile Map index (i,j) Exclude the border tiles from evaluation
  for (int i = 1; i < Size-1; ++i)
  {
    for (int j = 1; j < Size-1; ++j)
    {
      int Position = i * Size + j;

      std::array<glm::vec4, 9> Kernal = { glm::vec4(0) };
      bool isPeak = true;

      // Kernal index
      for (int k = 0; k < 9; ++k)
      {
        // Ignore trying to check if the current tile is a slope of the current tile
        if (k == 4)
        {
          continue;
        }

        int NeighborPositionI = i + RowOffsets[k];
        int NeighborPositionJ = j + ColumnOffsets[k];
        int NeighborPosition = NeighborPositionI * Size + NeighborPositionJ;

        // Check if neighbors are a lower height than the current tile
        if (TileMap[i][j] > TileMap[NeighborPositionI][NeighborPositionJ])
        {
          Kernal[k] = glm::vec4(RowOffsets[k], ColumnOffsets[k], 0, 1);
          //glm::vec4 a(0.5f, 0.5f, 0.0f, 1.0f);
          //a = Kernal[k] * a;
          //Kernal[k] = a;
        }
        else
        {
          isPeak = false;
        }
      }
      GradientMap[i][j] = Kernal;
      if (isPeak)
      {
        Peaks.push_back(Position);
      }
    }
  }
  return;

  //TODO: Add a dijksa search method for coloring the tiles closest to a given peak
  // Tile Map index (i,j) Exclude the border tiles from evaluation
  for (const int& Position : Peaks)
  {
    std::deque<int> Neighbors;
    Neighbors.push_back(Position);
    while (!Neighbors.empty()) // Preform a search around the unvisited land tile to visit the entire connected island
    {
      int Position = Neighbors.front();
      int PositionI = Position / Size;
      int PositionJ = Position % Size;
      Neighbors.pop_front();

      // Skip any duplicate tiles that were added
      if (VisitMap[PositionI][PositionJ] != 0)
      {
        continue;
      }
      // VisitMap[PositionI][PositionJ] = IslandCount; // Mark the current tile as visited

      // Look for any neighboring land tiles
      for (int k = 0; k < 9; ++k)
      {
        int NeighborPositionI = PositionI + RowOffsets[k];
        int NeighborPositionJ = PositionJ + ColumnOffsets[k];
        int NeighborPosition = NeighborPositionI * Size + NeighborPositionJ;

        // Check that the current position is not out of bounds
        if (CHECK_LEFT(NeighborPosition))
        {
          continue;
        }
        else if (CHECK_RIGHT(NeighborPosition))
        {
          continue;
        }

        if (CHECK_UP(NeighborPosition))
        {
          continue;
        }
        else if (CHECK_DOWN(NeighborPosition))
        {
          continue;
        }

        // Only add unvisited land tiles
        if (VisitMap[NeighborPositionI][NeighborPositionJ] == 0 && TileMap[NeighborPositionI][NeighborPositionJ] >= BEACH)
        {
          Neighbors.push_back(NeighborPosition);
        }
      }
    }
  }
}

void Blind::World::ErodeAndDeposit()
{
  int Position = WorldRNG.GenerateRandomInt((Size - 1) * (Size - 1));

  for(int i = 0; i < 10000; ++i)
  {
  }
}

void Blind::World::SaveMap(std::string name)
{
  TileFile.open(name + ".txt", std::ios::out);

  if (TileFile.is_open())
  {
    for (int i = 0; i < Size; ++i)
    {
      for (int j = 0; j < Size; ++j)
      {
        TileFile << std::setw(4) << TileMap[i][j];
      }
      TileFile << std::endl;
    }
    TileFile << std::endl;
    TileFile.close();
  }
}

void Blind::World::SaveMapImage(std::string name, int size, Blind::World::PalletType type, unsigned ColorShift)
{
  unsigned char* Pixel_buffer = new unsigned char[size * size * 3];

  int index = 0;
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      glm::vec3 Color = GetTileColor(i, j, type); // Get the color values of a tile

      // Convert it into a byte for the pixel buffer
      unsigned char Ri = static_cast<unsigned char>(std::max(0.0f, Color.r) * 255.0f);
      unsigned char Gi = static_cast<unsigned char>(std::max(0.0f, Color.g) * 255.0f);
      unsigned char Bi = static_cast<unsigned char>(std::max(0.0f, Color.b) * 255.0f);

      ColorShift = std::min(7u, ColorShift);

      Ri >>= ColorShift;
      Ri <<= ColorShift;

      Gi >>= ColorShift;
      Gi <<= ColorShift;

      Bi >>= ColorShift;
      Bi <<= ColorShift;

      // Add the value to the buffer for each channel
      Pixel_buffer[index++] = Ri;
      Pixel_buffer[index++] = Gi;
      Pixel_buffer[index++] = Bi;
    }
  }

  std::string Name("height_C_" + name + ".png");
  stbi_write_png(&Name[0], size, size, 3, Pixel_buffer, 3 * size);
}

void Blind::World::ReadMap(std::string name)
{
  std::ifstream File(name, std::ios::in);

  if (File.is_open())
  {
    for (int i = 0; i < Size; ++i)
    {
      for (int j = 0; j < Size; ++j)
      {
        File >> TileMap[i][j];
      }
    }
    File.close();
  }
}


float Blind::World::Fade(float t)
{
  return ((((6 * t - 15) * t + 10) * t) * t) * t;
}


float Blind::World::Lerp(float t, float x, float y)
{
  return x + t * (y - x);
}

/// <summary>
/// Constructor for a Mole class. All Moles start at the center of their map
/// </summary>
/// <param name="seed">Starting value to use in the RNG</param>
/// <param name="size">Size of the map this Mole opperates on</param>
Blind::Mole::Mole(int seed, int size) : Seed(seed), Size(size)
{
  //Seed = 1623503266; // Only for testing
  printf("%d\n", Seed);
  RNG = Random(Seed);

  CenterPosition();
}

/// <summary>
/// Assignment operator
/// </summary>
/// <param name="mole">Refrence to a separate Mole object</param>
/// <returns>Return the modified self</returns>
Blind::Mole& Blind::Mole::operator=(const Mole& mole)
{
  RNG = mole.RNG;
  Position = mole.Position;
  Seed = mole.Seed;
  Size = mole.Size;

  return *this;
}

/// <summary>
/// Private member. Generates a value between max and min inclusive
/// </summary>
/// <param name="max">Largest value this function can return</param>
/// <param name="min">Smallest value this function can return. Default value 0</param>
/// <returns>A random number within the given range</returns>
Blind::DIRECTION Blind::Mole::RandomDirection(int max, int min)
{
  return static_cast<DIRECTION>(RNG.GenerateRandomInt(max, min));
}

/// <summary>
/// Sets the Mole's position to the center of the associated map
/// </summary>
void Blind::Mole::CenterPosition()
{
  Position = ((Size / 2) * Size) + (Size / 2);
}

/// <summary>
/// Sets the Mole's position to a random spot anywhere on the map
/// </summary>
void Blind::Mole::RandomPosition()
{
  Position = RandomDirection((Size * Size) - 1);
}

/// <summary>
/// Moves the Mole randomly in one of the four possible directions. If the Mole 
/// </summary>
/// <returns></returns>
int Blind::Mole::RandomDirection()
{
  // Pick which direction the Mole will move
  DIRECTION Direction = RandomDirection(3);
  switch (Direction)
  {
  case UP:
  {
    MOLE_MOVE_UP;
    break;
  }
  case DOWN:
  {
    MOLE_MOVE_DOWN;
    break;
  }
  case LEFT:
  {
    MOLE_MOVE_LEFT;
    break;
  }
  case RIGHT:
  {
    MOLE_MOVE_RIGHT;
    break;
  }
  default:
    break;
  }

  // Make sure we are not about to leave the bounds of the map
  if (CHECK_LEFT(Position))
  {
    MOLE_MOVE_RIGHT;
  }
  else if (CHECK_RIGHT(Position))
  {
    MOLE_MOVE_LEFT;
  }

  if (CHECK_UP(Position))
  {
    MOLE_MOVE_DOWN;
  }
  else if (CHECK_DOWN(Position))
  {
    MOLE_MOVE_UP;
  }

  return Position;
}

Blind::Island::Island() : ID(0), Size(0), Color(0.0f)
{
  Bounds[UP] = -1;
  Bounds[DOWN] = std::numeric_limits<int>::max();
  Bounds[LEFT] = std::numeric_limits<int>::max();
  Bounds[RIGHT] = -1;
}

Blind::Island::Island(int id, glm::vec3 color) : ID(id), Size(0), Color(color)
{
  Bounds[UP] = -1;
  Bounds[DOWN] = std::numeric_limits<int>::max();
  Bounds[LEFT] = std::numeric_limits<int>::max();
  Bounds[RIGHT] = -1;
}

Blind::Island::~Island()
{

}

glm::vec3 Blind::Island::GetColor() const
{
  return Color;
}

int Blind::Island::GetBoundsAtIndex(Blind::DIRECTION index) const
{
  return Bounds[index];
}

