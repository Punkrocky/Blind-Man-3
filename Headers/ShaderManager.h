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

class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager();

  void Init();

  GLuint GetShaderID(int index)
  {
    return ShaderArray[index].GetShaderLocation();
  }
private:
  std::vector<Shader> ShaderArray;

  // functions
  void BuildShader(std::string vertexShader, std::string fragmentShader = "");
};
