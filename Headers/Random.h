/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 6-19-2020
 * File: Random.h
 **********************************************************************************************************************/
#pragma once

#include "Timer.h"

#include <random>

class Random
{
public:
  Random();
  Random(int seed);
  ~Random();

  int GenerateRandomInt(int max, int min = 0);
  float GenerateRandomFloat(float max, float min = 0.0f);


private:
  std::default_random_engine RandomGenerator;
  int Seed;
};
