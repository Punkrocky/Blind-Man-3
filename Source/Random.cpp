/*******************************************************************************************************************//**
 * \file Random.h
 * \author Matthew LaDouceur
 * \date 6-19-2020
 * \brief 
 **********************************************************************************************************************/
#include "Random.hpp"

Random::Random()
{
  Seed = std::chrono::system_clock::now().time_since_epoch().count();
  RandomGenerator.seed(Seed);
}

Random::Random(int seed)
{
  RandomGenerator.seed(seed);
}

Random::~Random()
{

}

int Random::GenerateRandomInt(int max, int min)
{
  std::uniform_int_distribution<int> Distribution(min, max);
  return Distribution(RandomGenerator);
}

float Random::GenerateRandomFloat(float max, float min)
{
  std::uniform_real_distribution<float> Distribution(min, max);
  return Distribution(RandomGenerator);
}
