/*******************************************************************************************************************//**
 * \file GraphicsComponent.h
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.h"
#include "Serializer.h"

#define VERTEX_COUNT 4
#define INDEX_COUNT 6

#define TEXTURE_PATH "../../Assets/Textures/"
#define SHADER_PATH "../../Assets/Shaders/"

// TODO [2]: Create a mesh manager
class Mesh
{
public:
  Mesh();
  Mesh(const Mesh& rhs);
  ~Mesh();

  glm::mat4& GetVertexPositions();
  glm::mat4& GetVertexColors();
  glm::mat4x2& GetVertexTextureCoords();
  std::vector<unsigned int>& GetIndices();

  void SetVertexColors(const glm::vec3& color);

private:
  glm::mat4 VertexPositions;         // XYZW
  glm::mat4 VertexColors;            // RGBA
  glm::mat4x2 VertexTextureCoords;   // UV
  std::vector<unsigned int> Indices; // Index into the above data
};

class Texture
{
public:
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
  ~Texture();

  std::string GetFileName() const;
  GLuint GetTextureLocation() const;

  void SetTextureLocation();

  pt::ptree Save() const;
  void Load(pt::ptree node);
  

private:
  std::string FileName;   // Relative location of the texture
  GLuint TextureLocation; // GPU memory address of the texture
  TextureType Type;       // The type of texture
};

class Shader
{
public:
  enum class ShaderType
  {
    INVALID_SHADER_TYPE = -1,
    Basic_s,
    SHADER_TYPE_TOTAL
  };

  Shader(std::string vertexFile = SHADER_PATH"BasicShader.vs", std::string fragmentFile = SHADER_PATH"BasicShader.fs");
  ~Shader();

  GLuint GetShaderLocation() const;
  std::string GetVertexFile() const;
  std::string GetFragmentFile() const;

  void SetShaderLocation();

  pt::ptree Save() const;
  void Load(pt::ptree node);


private:
  std::string VertexFile;   // Relative location of the vertex shader
  std::string FragmentFile; // Relative location of the fragment shader
  GLuint ShaderLocation;    // GPU memory address of the shader program
  ShaderType Type;          // The type of shader program
};

class GraphicsComponent
{
public:
  GraphicsComponent(Texture::TextureType typeTexture = Texture::TextureType::Default_t, 
                    Shader::ShaderType typeShader = Shader::ShaderType::Basic_s);
  GraphicsComponent(const GraphicsComponent& rhs);
  ~GraphicsComponent();

  // Getters
  Mesh* GetMesh();
  Texture::TextureType GetTextureType();
  Shader::ShaderType GetShaderType();

  // Setters
  void SetGraphicsComponentColor(glm::vec3 color);

private:
  Mesh* mesh;
  Texture::TextureType textureType;
  Shader::ShaderType shaderType;
};

typedef GraphicsComponent* GraphicsComponentPtr;