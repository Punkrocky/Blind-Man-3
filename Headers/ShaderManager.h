/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 4-28-2020
 * File: GraphicsComponent.h
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
