/*******************************************************************************************************************//**
 * \file 
 * \author Matthew LaDouceur
 * \date 
 * \brief 
 **********************************************************************************************************************/
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ostream>

inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
  os << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]\n";
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
  os << glm::vec4(vec, 0.0f);
  return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
  os << glm::vec3(vec, 0.0f);
  return os;
}
