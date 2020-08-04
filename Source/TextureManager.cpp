/***********************************************************************************************************************
 * Project: Blind Man 3
 * Autor: Matthew LaDouceur
 * Date: 7-27-2020
 * File: TextureManager.cpp
 **********************************************************************************************************************/

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <string>

#include "TextureManager.h"
#include "Serializer.h"

TextureManager::TextureManager() {}
TextureManager::~TextureManager() {}


void TextureManager::Init()
{
  Serializer TextureSerial(TEXTURE_PATH"TextureList.json", Serializer::Mode::Read);
  Texture TempTexture;

  while (TextureSerial.ReadJSON<Texture>(TempTexture))
  {
    BuildTexture(TempTexture);
  }
}


/**********************************************************************************************************************\
|*************************************************| PRIVATE MEMBERS |**************************************************|
\**********************************************************************************************************************/

void TextureManager::BuildTexture(Texture& texture)
{
  glm::ivec3 MetaData;
  unsigned char * TextureData = stbi_load(&texture.GetFileName()[0], &MetaData.x, &MetaData.y, &MetaData.z, STBI_rgb_alpha);

  if (TextureData != NULL)
  {
    texture.SetTextureLocation();
    glBindTexture(GL_TEXTURE_2D, texture.GetTextureLocation());

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MetaData.x, MetaData.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(TextureData);

    TextureList.emplace_back(texture);
  }
}

