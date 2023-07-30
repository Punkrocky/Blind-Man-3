/*******************************************************************************************************************//**
 * \file ShaderManager.h
 * \author Matthew LaDouceur
 * \date 5-27-2020
 * \brief
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "Serializer.h"
#include "OpenGLIncludes.h"

#define SHADER_PATH "../../Assets/Shaders/"

 /// Data related to a shader program
class Shader
{
public:
  /// Used to communicate with the ShaderManager
  enum class ShaderType
  {
    INVALID_SHADER_TYPE = -1,
    Basic_s,
    Line_s,
    SHADER_TYPE_TOTAL
  };

  Shader(std::string vertexFile = SHADER_PATH"BasicShader.vs", std::string fragmentFile = SHADER_PATH"BasicShader.fs");
  ~Shader() = default;

  GLuint GetShaderLocation() const;
  std::string GetVertexFile() const;
  std::string GetFragmentFile() const;

  void SetShaderLocation();

  pt::ptree Save() const;
  void Load(pt::ptree node);


private:
  ShaderType Type;          //!< The type of shader program
  GLuint ShaderLocation;    //!< GPU memory address of the shader program
  std::string VertexFile;   //!< Relative location of the vertex shader
  std::string FragmentFile; //!< Relative location of the fragment shader
};


// Handles the allocation and deallocation of Shader programs
class ShaderManager
{
public:
  ShaderManager();
  ~ShaderManager() = default;

  GLuint GetShaderID(Shader::ShaderType index) const;
  Shader* GetShaderPtr(Shader::ShaderType type);

private:
  // Functions
  void BuildShader(Shader& shader);

  // Variables
  std::vector<Shader> ShaderArray;
};

typedef Shader* ShaderPtr;
typedef ShaderManager* ShaderManagerPtr;
typedef Shader::ShaderType SType;
