/*******************************************************************************************************************//**
 * \file Logging.h
 * \author Matthew LaDouceur
 * \date 7-31-2023
 * \brief Header for creating log files
 **********************************************************************************************************************/
#pragma once

#include <iostream>
#include <fstream>

class LogFile
{
public:
  LogFile(std::string filename = "Log");
  ~LogFile();

  friend std::ostream& operator<<(std::ostream& os, const std::string& message);
  friend std::ostream& operator<<(LogFile& obj, const std::string& message);

private:
  std::fstream File;
};