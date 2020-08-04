/*******************************************************************************************************************//**
 * \file ShaderManager.cpp
 * \author Matthew LaDouceur
 * \date 5-27-2020
 * \brief
 **********************************************************************************************************************/
#include <iostream>
#include <fstream>
#include <sstream>

#include "ShaderManager.h"

ShaderManager::ShaderManager() {}
ShaderManager::~ShaderManager() {}


void ShaderManager::Init()
{
  Serializer ShaderReader(SHADER_PATH"ShaderList.json", Serializer::Mode::Read);

  Shader TempShader;
  while (ShaderReader.ReadJSON(TempShader))
  {
    BuildShader(TempShader);
  }
}

/**********************************************************************************************************************\
|*************************************************| PRIVATE MEMBERS |**************************************************|
\**********************************************************************************************************************/

void ShaderManager::BuildShader(Shader& shader)
{
  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(shader.GetVertexFile(), std::ios::in);
  if (VertexShaderStream.is_open())
  {
    std::stringstream sstr;
    sstr << VertexShaderStream.rdbuf();
    VertexShaderCode = sstr.str();
    VertexShaderStream.close();
  }
  else
  {
    printf("Impossible to open %s. Are you in the right directory?\n", &shader.GetVertexFile()[0]);
    getchar();
    return;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(shader.GetFragmentFile(), std::ios::in);
  if (FragmentShaderStream.is_open())
  {
    std::stringstream sstr;
    sstr << FragmentShaderStream.rdbuf();
    FragmentShaderCode = sstr.str();
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  printf("Compiling shader : %s\n", &shader.GetVertexFile()[0]);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling shader : %s\n", &shader.GetFragmentFile()[0]);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }


  // Link the program
  printf("Linking program\n");
  //GLuint ProgramID = glCreateProgram();
  shader.SetShaderLocation();
  glAttachShader(shader.GetShaderLocation(), VertexShaderID);
  glAttachShader(shader.GetShaderLocation(), FragmentShaderID);
  glLinkProgram(shader.GetShaderLocation());

  // Check the program
  glGetProgramiv(shader.GetShaderLocation(), GL_LINK_STATUS, &Result);
  glGetProgramiv(shader.GetShaderLocation(), GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0)
  {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(shader.GetShaderLocation(), InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }


  glDetachShader(shader.GetShaderLocation(), VertexShaderID);
  glDetachShader(shader.GetShaderLocation(), FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  ShaderArray.emplace_back(shader);
}

