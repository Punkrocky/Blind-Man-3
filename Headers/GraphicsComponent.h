/*******************************************************************************************************************//**
 * \file GraphicsComponent.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Header for Mesh, Texture, Shader, and GraphicsComponent
 **********************************************************************************************************************/
#pragma once
#include <vector>

#include "OpenGLIncludes.h"
#include "Serializer.h"

#define VERTEX_COUNT 4
#define INDEX_COUNT 6

#define TEXTURE_PATH "../../Assets/Textures/"
#define SHADER_PATH "../../Assets/Shaders/"

/// Data related to mesh objects
class Mesh
{
public:
  Mesh();
  Mesh(const Mesh& rhs);
  ~Mesh() = default;

  glm::mat4& GetVertexPositions();
  glm::mat4x2& GetVertexTextureCoords();
  std::vector<unsigned int>& GetIndices();

private:
  glm::mat4 VertexPositions;         //!< XYZW. Each column of the matrix is a vertex of data
  glm::mat4x2 VertexTextureCoords;   //!< UV. Each column of the matrix is a vertex of data
  std::vector<unsigned int> Indices; //!< Index into the above vertices
};

/// Data related to a texture
class Texture
{
public:
  /// Used to comunicate with the TextureManager
  enum class TextureType
  {
    INVALID_TEXTURE_TYPE = -1,
    Default_t,
    Grass_t,
    Tree_t,
    Farm_t,
    TEXTURE_TYPE_TOTAL
  };

  Texture(std::string fileName = TEXTURE_PATH"000_Default.png");
  ~Texture() = default;

  std::string GetFileName() const;
  GLuint GetTextureLocation() const;

  void SetTextureLocation();

  pt::ptree Save() const;
  void Load(pt::ptree node);
  

private:
  std::string FileName;   //!< Relative location of the texture
  GLuint TextureLocation; //!< GPU memory address of the texture
  TextureType Type;       //!< The type of texture
};

/// Data related to a shader program
class Shader
{
public:
  /// Used to communicate with the ShaderManager
  enum class ShaderType
  {
    INVALID_SHADER_TYPE = -1,
    Basic_s,
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
  std::string VertexFile;   //!< Relative location of the vertex shader
  std::string FragmentFile; //!< Relative location of the fragment shader
  GLuint ShaderLocation;    //!< GPU memory address of the shader program
  ShaderType Type;          //!< The type of shader program
};

/// Holds refrence data for Texture, Shader, and the colors of an attached entity
class GraphicsComponent
{
public:
  GraphicsComponent(Texture::TextureType typeTexture = Texture::TextureType::Default_t,
                    float textureIndex = 0.0f, Shader::ShaderType typeShader = Shader::ShaderType::Basic_s);
  GraphicsComponent(const GraphicsComponent& rhs);
  ~GraphicsComponent() = default;

  // Getters
  Texture::TextureType GetTextureType();
  Shader::ShaderType GetShaderType();
  glm::vec4 GetColor();
  float GetTextureIndex();

  // Setters
  void SetGraphicsComponentColor(glm::vec3 color);

private:
  glm::vec4 Color;                  //!< A multiplicative color to be applied on top of the attached texture
  float TexuteIntex;                //!< An index related to the TextureType for batch rendering
  Texture::TextureType textureType; //!< A refrence to the attached Texture
  Shader::ShaderType shaderType;    //!< A refrence to the attached Shader
};

typedef GraphicsComponent* GraphicsComponentPtr;