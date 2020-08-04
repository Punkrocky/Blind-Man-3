/*******************************************************************************************************************//**
 * \file ShaderManager.h
 * \author Matthew LaDouceur
 * \date 5-27-2020
 * \brief
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.h"
#include "GraphicsComponent.h"

#define SHADER_PATH "../../Assets/Shaders/"

class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager();

  void Init();

  GLuint GetShaderID(Shader::ShaderType index)
  {
    return ShaderArray[static_cast<int>(index)].GetShaderLocation();
  }
private:
  // Functions
  void BuildShader(Shader& shader);

  // Variables
  std::vector<Shader> ShaderArray;
};
