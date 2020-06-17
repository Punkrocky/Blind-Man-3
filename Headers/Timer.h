/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 6-12-2020
 * File: Timer.h
 **********************************************************************************************************************/
#pragma once

#include <chrono>

class Timer
{
public:
  Timer();
  ~Timer();

  void StartFrame();
  float EndFrame();


private:
  std::chrono::steady_clock Clock;
  std::chrono::time_point<std::chrono::steady_clock> LastTimePoint;
  std::chrono::duration<float, std::ratio<1,1>> FrameTime;
};
