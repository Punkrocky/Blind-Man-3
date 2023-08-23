/*******************************************************************************************************************//**
 * \file TextureManager.cpp
 * \author Matthew LaDouceur
 * \date 7-27-2020
 * \brief Source for TextureManager
 **********************************************************************************************************************/
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <string>

#include "TextureManager.hpp"

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



 /*!
  * \brief Reads a JSON file for texture names and builds them on the GPU
  *
  */
TextureManager::TextureManager()
{
  Serializer TextureSerial(TEXTURE_PATH"TextureList.json", Serializer::Mode::Read);
  Texture TempTexture;

  while (TextureSerial.ReadJSON<Texture>(TempTexture))
  {
    BuildTexture(TempTexture);
  }
}

/*!
 * \brief Access texture data from the private array
 *
 * \param index
 *  The type of texture we are looking to retriev from the array
 *
 * \return
 *  The GPU memory address related to the texture
 *
 */
GLuint TextureManager::GetTextureID(Texture::TextureType index) const
{
  return TextureArray[static_cast<int>(index)].GetTextureLocation();
}


Texture* TextureManager::GetTexturePtr(Texture::TextureType type)
{
  return &TextureArray[static_cast<int>(type)];
}

/**********************************************************************************************************************\
|*************************************************| PRIVATE MEMBERS |**************************************************|
\**********************************************************************************************************************/

/*!
 * \brief Access texture data from the private array
 *
 * \param texture
 *  A Texture object to store data and copy into the TextureList
 *
 */
void TextureManager::BuildTexture(Texture& texture)
{
  glm::ivec3 MetaData;
  unsigned char * TextureData = stbi_load(&texture.GetFileName()[0], &MetaData.x, &MetaData.y, &MetaData.z, STBI_rgb_alpha);

  // Make sure we were able to read the texture
  if (TextureData != NULL)
  {
    // Generate a GPU address for the texture
    texture.SetTextureLocation();
    glBindTexture(GL_TEXTURE_2D, texture.GetTextureLocation());

    // Move the texture data to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MetaData.x, MetaData.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Free the texture data from the CPU
    stbi_image_free(TextureData);

    TextureArray.emplace_back(texture);
  }
}

