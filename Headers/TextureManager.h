/*******************************************************************************************************************//**
 * \file TextureManager.h
 * \author Matthew LaDouceur
 * \date 7-27-2020
 * \brief Header for TextureManager
 **********************************************************************************************************************/
#pragma once
#include <vector>

#include "GraphicsComponent.h"

#define TEXTURE_PATH "../../Assets/Textures/"

/// Handles the allocation and deallocation of Texture objects
class TextureManager
{
public:
  TextureManager() = default;
  ~TextureManager() = default;

  void Init();
  GLuint GetTextureID(Texture::TextureType index);

private:
  void BuildTexture(Texture& texture);

  std::vector<Texture> TextureList; //!< Holds all the textures used in the game
};
