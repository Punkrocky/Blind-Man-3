/*******************************************************************************************************************//**
 * \file GraphicsComponent.cpp
 * \author Matthew LaDouceur
 * \date 4-28-2020
 * \brief Source for Mesh, Texture, Shader, and GraphicsComponent
 **********************************************************************************************************************/
#include "GraphicsComponent.h"

#pragma region Mesh

Mesh::Mesh()
{
  // Position Data
  VertexPositions[0] = glm::vec4( 1.0f,  1.0f, 0.0f, 1.0f); // Top Right
  VertexPositions[1] = glm::vec4( 1.0f, -1.0f, 0.0f, 1.0f); // Bottom Right
  VertexPositions[2] = glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f); // Bottom Left
  VertexPositions[3] = glm::vec4(-1.0f,  1.0f, 0.0f, 1.0f); // Top Left

  // Texture Data
  VertexTextureCoords[0] = glm::vec2(1.0f, 0.0f); // Bottom Right
  VertexTextureCoords[1] = glm::vec2(1.0f, 1.0f); // Top Right
  VertexTextureCoords[2] = glm::vec2(0.0f, 1.0f); // Top Left
  VertexTextureCoords[3] = glm::vec2(0.0f, 0.0f); // Bottom Left

  Indices.resize(INDEX_COUNT);
  // Triangle 1
  Indices[0] = 0;
  Indices[1] = 3;
  Indices[2] = 2;
  // Triangle 2
  Indices[3] = 2;
  Indices[4] = 1;
  Indices[5] = 0;
}


Mesh::Mesh(const Mesh& rhs)
{
  this->VertexPositions = rhs.VertexPositions;
  this->VertexTextureCoords = rhs.VertexTextureCoords;
  this->Indices = rhs.Indices;
}



glm::mat4& Mesh::GetVertexPositions()
{
  return VertexPositions;
}


glm::mat4x2& Mesh::GetVertexTextureCoords()
{
  return VertexTextureCoords;
}


std::vector<unsigned int>& Mesh::GetIndices()
{
  return Indices;
}

#pragma endregion

#pragma region Texture

Texture::Texture(std::string fileName) : FileName(fileName) {}



std::string Texture::GetFileName() const
{
  return FileName;
}


GLuint Texture::GetTextureLocation() const
{
  return TextureLocation;
}


void Texture::SetTextureLocation()
{
  glGenTextures(1, &TextureLocation);
}

pt::ptree Texture::Save() const
{
  pt::ptree node;

  node.add("Texture Type", static_cast<int>(Type));
  node.add("File Name", FileName);

  return node;
}


void Texture::Load(pt::ptree node)
{
  Type = static_cast<TextureType>(node.get<int>("Texture Type"));
  FileName = TEXTURE_PATH + node.get<std::string>("File Name");
}


#pragma endregion

#pragma region Shader

Shader::Shader(std::string vertexFile, std::string fragmentFile) : VertexFile(vertexFile), FragmentFile(fragmentFile) {}


GLuint Shader::GetShaderLocation() const
{
  return ShaderLocation;
}


std::string Shader::GetVertexFile() const
{
  return VertexFile;
}


std::string Shader::GetFragmentFile() const
{
  return FragmentFile;
}


void Shader::SetShaderLocation()
{
  ShaderLocation = glCreateProgram();
}


pt::ptree Shader::Save() const
{
  pt::ptree node;

  node.add("Shader Type", static_cast<int>(Type));
  node.add("Vertex Shader", VertexFile);
  node.add("Fragment Shader", FragmentFile);

  return node;
}


void Shader::Load(pt::ptree node)
{
  Type = static_cast<ShaderType>(node.get<int>("Shader Type"));
  VertexFile = SHADER_PATH + node.get<std::string>("Vertex Shader");
  FragmentFile = SHADER_PATH + node.get<std::string>("Fragment Shader");
}

#pragma endregion

#pragma region GraphicsComponent

GraphicsComponent::GraphicsComponent(Texture::TextureType typeTexture, Shader::ShaderType typeShader)
{
  textureType = typeTexture;
  shaderType = typeShader;
}


GraphicsComponent::GraphicsComponent(const GraphicsComponent& rhs)
{
  this->Color = rhs.Color;
  this->textureType = rhs.textureType;
  this->shaderType = rhs.shaderType;
}


Texture::TextureType GraphicsComponent::GetTextureType()
{
  return textureType;
}


Shader::ShaderType GraphicsComponent::GetShaderType()
{
  return shaderType;
}


glm::vec4 GraphicsComponent::GetColor()
{
  return Color;
}


void GraphicsComponent::SetGraphicsComponentColor(glm::vec3 color)
{
  Color = glm::vec4(color, 1.0f);
}

#pragma endregion
