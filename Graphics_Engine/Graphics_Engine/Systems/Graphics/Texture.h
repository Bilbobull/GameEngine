#pragma once
#include "Graphics_Headers.h"
#include <string>

class Texture
{
public:
  Texture(std::string filename);
  void Bind(void);
  void unBind(void);
  bool YesorNo(void){ if (format != NULL) return true;
                      else return false; }
private:
  std::string file;
  int width;
  int height;
  int numChannels;
  GLenum format;
  unsigned char* pixels;
  GLuint texture;
  bool hasAlpha;

  void createGLTexture();
};