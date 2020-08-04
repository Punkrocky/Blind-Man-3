/*******************************************************************************************************************//**
 * \file TextureManager.h
 * \author Matthew LaDouceur
 * \date 7-27-2020
 * \brief
 **********************************************************************************************************************/
#pragma once

#include <vector>

#include "GraphicsComponent.h"

#define TEXTURE_PATH "../../Assets/Textures/"

class TextureManager
{
public:
  TextureManager();
  ~TextureManager();

  void Init();

  GLuint GetTextureID(Texture::TextureType index)
  {
    return TextureList[static_cast<int>(index)].GetTextureLocation();
  }

private:
  void BuildTexture(Texture& texture);

  std::vector<Texture> TextureList;
};
