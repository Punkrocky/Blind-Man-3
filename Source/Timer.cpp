/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 6-12-2020
 * File: Timer.cpp
 **********************************************************************************************************************/

#include "Timer.h"

Timer::Timer()
{
  LastTimePoint = Clock.now();
}

Timer::~Timer()
{

}

void Timer::StartFrame()
{
  LastTimePoint = Clock.now();
}

float Timer::EndFrame()
{
  std::chrono::time_point<std::chrono::steady_clock> NewestTimePoint = Clock.now();
  FrameTime = NewestTimePoint - LastTimePoint;
  return FrameTime.count();
}
