/*******************************************************************************************************************//**
 * \file TextureManager.h
 * \author Matthew LaDouceur
 * \date 7-27-2020
 * \brief Header for TextureManager
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "OpenGLIncludes.hpp"
#include "Serializer.hpp"

#define TEXTURE_PATH "../../Assets/Textures/"

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
    White_t,
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
  TextureType Type;       //!< The type of texture
  GLuint TextureLocation; //!< GPU memory address of the texture
  std::string FileName;   //!< Relative location of the texture
};


/// Handles the allocation and deallocation of Texture objects
class TextureManager
{
public:
  TextureManager();
  ~TextureManager() = default;

  GLuint GetTextureID(Texture::TextureType index) const;
  Texture* GetTexturePtr(Texture::TextureType type);

private:
  void BuildTexture(Texture& texture);

  std::vector<Texture> TextureArray; //!< Holds all the textures used in the game
};

typedef Texture* TexturePtr;
typedef TextureManager* TextureManagerPtr;
typedef Texture::TextureType TType;
