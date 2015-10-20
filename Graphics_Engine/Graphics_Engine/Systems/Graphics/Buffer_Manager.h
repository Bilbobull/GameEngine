// Author: Gonzalo Rojo

#pragma once

#include "Graphics_Headers.h"
#include <vector>

class VertexArrayObject
{
public:
  VertexArrayObject(void)
  {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
  }

  ~VertexArrayObject(void)
  {
    glDeleteVertexArrays(1, &vao);
    vao = 0;
  }

  void Bind(void)
  {
    glBindVertexArray(vao);
  }

  void unBind(void)
  {
    glBindVertexArray(0);
  }

private:
  GLuint vao;
};


class VertexBufferObject
{
public:
  VertexBufferObject(void)
  {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STREAM_DRAW);
  }

  ~VertexBufferObject(void)
  {
    glDeleteBuffers(1, &vbo);
    vbo = 0;
  }

  VertexBufferObject(int size, void* data)
  {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);

    Num_Vertices = (size / 2) / sizeof(float);
  }

  void Bind(void)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
  }

  void unBind(void)
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Add_Data(float* data, GLuint size)
  {
    Data.insert(Data.end(), data, data + size);
  }

  void Upload_Data(GLenum mode)
  {
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, Data.size() * sizeof(float), Data.data(), mode);
    Data.clear();
    this->unBind();
  }

  void Add_Buffer_Data(int size, void* data)
  {
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW);
  }

  int Get_Data_Size(void)
  {
    return Data.size();
  }

  int Get_Vertice_Count(void)
  {
    return Num_Vertices;
  }

private:
  GLuint vbo;
  int Num_Vertices;
  std::vector <float> Data;
};


class ElementBufferObject
{
public:
  ElementBufferObject(int size, GLushort* data)
  {
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, (const void*)data, GL_STATIC_DRAW);
  }

  ~ElementBufferObject()
  {
    glDeleteBuffers(1, &ebo);
    ebo = 0;
  }

  void Bind(void)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  }

  void unBind(void)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }


private:
  GLuint ebo;
};

class FrameBufferObject
{
public:
  FrameBufferObject(void)
  {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  }

  ~FrameBufferObject()
  {
    glDeleteFramebuffers(1, &fbo);
    fbo = 0;
  }

  void Bind(void)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  }

  void unBind(void)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Texture2D(GLuint texture)
  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
  }

private:
  GLuint fbo;
};


class ShaderStorageBuffer
{
public:
  ShaderStorageBuffer::ShaderStorageBuffer() : bufferPos(0) {}
  ShaderStorageBuffer(GLuint size, GLenum mode = GL_STATIC_DRAW)
  {
    glGenBuffers(1, &bufferPos);
    BindBuffer();
    BufferData(size, NULL, mode);
  }

  ~ShaderStorageBuffer(){}

  void GenerateBuffer()
  {
    glGenBuffers(1, &bufferPos);
  }

  void BindBuffer(GLenum mode = GL_SHADER_STORAGE_BUFFER)
  {
    glBindBuffer(mode, bufferPos);
  }

  void BindBufferBase(GLuint index = 0)
  {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, bufferPos);
  }

  void BufferData(GLuint size, const void* data = NULL, GLenum mode = GL_STATIC_DRAW)
  {
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, mode);
  }

  void BufferSubData(GLuint size, GLuint offset, const void* data)
  {
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
  }

  template <typename T>
  T* MapBufferRange(int offset, int size)
  {
    return (T*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset * sizeof(T), size * sizeof(T), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  }
  void UnMapBuffer()
  {
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  }

  void UnBindBuffer()
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  }

  void DeleteBuffer()
  {
    glDeleteBuffers(1, &bufferPos);
  }

  inline GLuint Get_POS()
  {
    return bufferPos;
  }
  bool IsBuffer()
  {
    if (glIsBuffer(bufferPos))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
private:
  GLuint bufferPos;
};

typedef ShaderStorageBuffer SSBO;
typedef VertexArrayObject VAO;
typedef VertexBufferObject VBO;
typedef ElementBufferObject EBO;
typedef FrameBufferObject FBO;