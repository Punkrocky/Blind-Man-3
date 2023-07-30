/*******************************************************************************************************************//**
 * \file Timer.h
 * \author Matthew LaDouceur
 * \date 6-12-2020
 * \brief
 **********************************************************************************************************************/
#pragma once

#include <chrono>
#include <ostream>

class Timer
{
public:
  Timer();
  ~Timer();

  friend std::ostream& operator<<(std::ostream& os, const Timer& T);

  void StartFrame();
  float EndFrame();


private:
  std::chrono::steady_clock Clock;
  std::chrono::time_point<std::chrono::steady_clock> LastTimePoint;
  std::chrono::duration<float, std::ratio<1,1>> FrameTime;
};
