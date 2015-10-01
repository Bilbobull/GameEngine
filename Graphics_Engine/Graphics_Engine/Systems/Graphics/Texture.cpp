#include "Texture.h"

Texture::Texture(std::string filename)
{
  file = filename + "_Diffuse.png";
  std::string path = "Textures\\" + file;
  
  pixels = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
  if (pixels)
  {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    createGLTexture();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }
}

void Texture::Bind(void)
{
  glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unBind(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::createGLTexture()
{
  format = numChannels == 4 ? GL_RGBA : GL_RGB;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
}