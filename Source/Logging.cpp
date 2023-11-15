/*******************************************************************************************************************//**
 * \file Logging.cpp
 * \author Matthew LaDouceur
 * \date 7-31-2023
 * \brief Source for creating log files
 **********************************************************************************************************************/
#include "Logging.hpp"
#include "Timer.hpp"

LogFile::LogFile(std::string filename)
{
  char buffer[27];
  buffer[0] = '_';

  std::time_t Date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  ctime_s((buffer+1), 26, &Date);

  // Replace newline and terminating null
  buffer[25] = '_';
  buffer[26] = '_';

  // Move year to appear after day date
  buffer[12] = buffer[21];
  buffer[13] = buffer[22];
  buffer[14] = buffer[23];
  buffer[15] = buffer[24];

  // Replace any spaces
  for (int i = 1; i < 27; ++i)
  {
    if (buffer[i] == ' ')
    {
      buffer[i] = '_';
    }
  }

  // Add the date, up to the year, to the filename
  filename.append(buffer, 16);
  filename.append(".txt");

  // Create or open the file
  File = std::fstream(filename, std::ios::out | std::ios::trunc);
  if (!File.is_open())
  {
    std::cerr << "Failed to open Log file\n";
  }
}

LogFile::~LogFile()
{
  if (File.is_open())
  {
    File.close();
  }
}


std::ostream& operator<<(std::ostream& os, const std::string& message)
{
  os << message.c_str();
  return os;
}

std::ostream& operator<<(LogFile& obj, const std::string& message)
{
  obj.File << message;
  return obj.File;
}
