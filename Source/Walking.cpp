/*******************************************************************************************************************//**
 * \file Walking.cpp
 * \author Matthew LaDouceur
 * \date 6-8-2021
 * \brief Source for the map generation
 **********************************************************************************************************************/

#include "Walking.h"

#include <STB/stb_image.h>
#ifndef STBI_MSC_SECURE_CRT
#define STBI_MSC_SECURE_CRT
#endif // !STBI_MSC_SECURE_CRT
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif // !STB_IMAGE_WRITE_IMPLEMENTATION
#include <STB/stb_image_write.h>

#define MAP_POS_INDEX TileMap[Position % Size][Position / Size]

#define MOLE_CHECK_LEFT Position % Size == 0
#define MOLE_CHECK_RIGHT Position % Size == Size - 1
#define MOLE_CHECK_DOWN Position >= (Size * (Size - 1)) - 1
#define MOLE_CHECK_UP Position < Size
#define MOLE_MOVE_LEFT --Position
#define MOLE_MOVE_RIGHT ++Position
#define MOLE_MOVE_DOWN Position += Size
#define MOLE_MOVE_UP Position -= Size


Blind::World::World(int size, int moleCount) : Size(size)
{
  MoleArray.reserve(moleCount);
  for (int i = 0; i < moleCount; ++i)
  {
    MoleArray.push_back(Mole(251 * i + Size, Size));
  }

  TileMap.resize(Size);
  for (int i = 0; i < Size; ++i)
  {
    TileMap[i].resize(Size);
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

void Blind::World::MAPGenerateGenOrder()
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

void Blind::World::MAPGenerateIslandMap()
{
  int floortiles = 0; // Count the number of altered tiles to stop generation
  int attempts = 0;   // Count the total number of generation attempts to prevent livelock

  do
  {
    int Position = 0;
    for (Blind::Mole& m : MoleArray)
    {
      // Tell the Mole to move
      Position = m.RandomDirection();

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
}

/// <summary>
/// Reset the entire map to 0's
/// </summary>
void Blind::World::CleanMap()
{
  std::memset(&TileMap[0], 0, Size * Size * sizeof(int));
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

void Blind::World::SaveMapImage(std::string name, int size)
{
  unsigned char* gray_pixels = new unsigned char[size * size * 3];

  int index = 0;
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      for (int k = 0; k < 3; ++k)
      {
        gray_pixels[index] = TileMap[i][j];
        ++index;
      }
    }
  }

  //stbi_write_png(&(name + ".png")[0], 1025, 1025, 3, color_pixels, 1025 * 3);
  //stbi_write_png(&("height_" + name + ".png")[0], 1025, 1025, 3, gray_pixels, 1025 * 3);

  //stbi_write_bmp(&("height_" + name + ".bmp")[0], 1025, 1025, 1, gray_pixels);

  std::string Name("height_" + name + ".bmp");
  stbi_write_bmp(&Name[0], size, size, 3, gray_pixels);

  /*stbi__write_context s = { 0 };
  if (stbi__start_write_file(&s, &Name[0]))
  {
    int pad = (-1025 * 3) & 3;
    stbiw__outfile(&s, -1, -1, 1025, 1025, 3, 0, (void*)gray_pixels, 1, pad,
                   "11 4 22 4" "4 44 22 444444",
                   'B', 'M', 14 + 40 + (1025 * 3 + pad) * 1025, 0, 0, 14 + 40,
                   40, 1025, -1025, 1, 24, 0, 0, 0, 0, 0, 0);

    stbi__end_write_file(&s);
  }*/
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


/// <summary>
/// Constructor for a Mole class. All Moles start at the center of their map
/// </summary>
/// <param name="seed">Starting value to use in the RNG</param>
/// <param name="size">Size of the map this Mole opperates on</param>
Blind::Mole::Mole(int seed, int size) : Seed(seed), Size(size)
{
  //Seed = 1623503267; // Only for testing
  printf("%d\n", Seed);
  Generator.seed(Seed);

  CenterPosition();
}

/// <summary>
/// Assignment operator
/// </summary>
/// <param name="mole">Refrence to a separate Mole object</param>
/// <returns>Return the modified self</returns>
Blind::Mole& Blind::Mole::operator=(const Mole& mole)
{
  Generator = mole.Generator;
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
Blind::Mole::DIRECTION Blind::Mole::RandomInt(int max, int min)
{
  std::uniform_int_distribution<int> Distribution(min, max);
  return static_cast<DIRECTION>(Distribution(Generator));
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
  Position = RandomInt((Size * Size) - 1);
}

/// <summary>
/// Moves the Mole randomly in one of the four possible directions. If the Mole 
/// </summary>
/// <returns></returns>
int Blind::Mole::RandomDirection()
{
  // Pick which direction the Mole will move
  DIRECTION Direction = RandomInt(3);
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
  if (MOLE_CHECK_LEFT)
  {
    MOLE_MOVE_RIGHT;
  }
  else if (MOLE_CHECK_RIGHT)
  {
    MOLE_MOVE_LEFT;
  }
  else if (MOLE_CHECK_DOWN)
  {
    MOLE_MOVE_UP;
  }
  else if (MOLE_CHECK_UP)
  {
    MOLE_MOVE_DOWN;
  }

  return Position;
}
