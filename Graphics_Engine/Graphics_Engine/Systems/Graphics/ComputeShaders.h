#pragma once

#include "Graphics_Headers.h"
#include <string>

namespace ComputeShaders
{

  class ComputeShader
  {
  public:
    ComputeShader(const char* cs);
    ~ComputeShader();

    std::string ReadComputeShader(const char* filename);
    void CreateComputeShader(const char* filename);
    GLuint Create_Program(std::string name, GLuint _computeShader);

    inline void Dispatch_Compute(GLuint workGroups_x, GLuint workGroups_y, GLuint workGroups_z)
    {
      glDispatchCompute(workGroups_x, workGroups_y, workGroups_z);
    }

     GLuint Get_ID();
     void Bind();
     void unBind();
     GLint attribLocation(const char* attrib);
     GLint uniLocation(const char *varName);

    // Useful glAttrib* methods
    void enableVertexAttribArray(GLint attr);
    void vertexAttribPtr(GLint index, GLint size, GLenum type, GLboolean normalized, GLint stride, GLint offset);
    void bindAttribLocation(GLint location, const char* attribName);
    // Useful glUniform* methods, returns false when no 'varName' found in shader

    bool uni1i(const char* varName, int value);
    bool uni1f(const char* varName, float value);
    bool uni2f(const char* varName, float x, float y);
    bool uni3f(const char* varName, float x, float y, float z);
    bool uni4f(const char* varName, float x, float y, float z, float w);
    bool uni1fv(const char* varName, GLuint count, float *values);
    bool uni2fv(const char* varName, const float* value);
    bool uni3fv(const char* varName, const float* values, const int count = 0);
    bool uni4fv(const char* varName, const float* values);

    //
    // uniform matrix
    //

    /// sends single 3x3 matrix
    bool uniMat3(const char *varName, float *mat, bool transpose = false);
    /// sends single 4x4 matrix
    bool uniMat4(const char *varName, const float*mat, bool transpose = false);

    GLuint shaderProgram;
    GLuint computeShader;
  };
}