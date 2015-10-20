#include "ComputeShaders.h"
#include <iostream>
#include <fstream>
#include <assert.h >

namespace ComputeShaders
{
  std::string ComputeShader::ReadComputeShader(const char* filename)
  {
    std::ifstream shaderFile(filename);

    if (!shaderFile.good())
    {
      std::cout << "File cannot be loaded...  " << filename;
      return NULL;
    }

    return std::string(std::istreambuf_iterator<char>(shaderFile),
      std::istreambuf_iterator<char>());
  }

  void ComputeShader::CreateComputeShader(const char* filename)
  {
    std::string shaderSource;
    shaderSource = ReadComputeShader(filename);
    const char* source = shaderSource.c_str();
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    char log[1000];
    glGetShaderInfoLog(shader, 1000, NULL, log);

    std::cout << filename << "\nCompile Status...\n" << log << "\n--------------" << std::endl;

    computeShader = shader;
  }

  ComputeShader::ComputeShader(const char* cs)
  {
    CreateComputeShader(cs);
    Create_Program("", computeShader);
  }


  ComputeShader::~ComputeShader()
  {
    glDetachShader(shaderProgram, computeShader);
    glDeleteShader(computeShader);
    glDeleteProgram(shaderProgram);
  }

  GLuint ComputeShader::Create_Program(std::string name, GLuint _computeShader /*= 0*/)
  {
    GLuint program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);

    char log[1000];
    glGetProgramInfoLog(program, 1000, 0, log);
    std::cout << "\n" << name << "\nLink Status...\n" << log << "\n--------------" << std::endl;

    shaderProgram = program;
    computeShader = _computeShader;

    return shaderProgram;
  }


  //////////////////////////////////////////////////////////////////////////
  // Inline Methods
  //////////////////////////////////////////////////////////////////////////

  GLint ComputeShader::attribLocation(const char* attribName)
  {
    GLint _attribLoc = glGetAttribLocation(shaderProgram, attribName);
    //
    //#ifdef _DEBUG
    //    if (_attribLoc == -1)
    //      std::cout << "ATTRIB " << attribName << " NOT FOUND!\n";
    //#endif

    return _attribLoc;
  }


  GLint ComputeShader::uniLocation(const char *varName)
  {
    assert(shaderProgram > 0 && "create the program id first!");
    GLint i = glGetUniformLocation(shaderProgram, varName);

    //    // log msg only in the DEBUG version
    //#ifdef _DEBUG
    //    if (i == -1)
    //      std::cout << "uniform" << varName << "does not exist!\n";
    //#endif

    return i;
  }

  void ComputeShader::bindAttribLocation(GLint location, const char* attribName)
  {
    glBindAttribLocation(shaderProgram, location, attribName);
  }


  void ComputeShader::enableVertexAttribArray(GLint attr)
  {
    glEnableVertexAttribArray(attr);
  }


  void ComputeShader::vertexAttribPtr(GLint index, GLint size, GLenum type, GLboolean normalized, GLint stride, GLint offset)
  {
    glVertexAttribPointer(index, size, type, normalized, stride, (const void*)offset);
  }


  // Useful Uniforms

  bool ComputeShader::uni1i(const char *varName, int value)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform1i(i, value);
    return true;
  }

  bool ComputeShader::uni1f(const char *varName, float value)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform1f(i, value);
    return true;
  }

  bool ComputeShader::uni2f(const char *varName, float x, float y)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform2f(i, x, y);
    return true;
  }

  bool ComputeShader::uni3f(const char *varName, float x, float y, float z)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform3f(i, x, y, z);
    return true;
  }

  bool ComputeShader::uni4f(const char *varName, float x, float y, float z, float w)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform4f(i, x, y, z, w);
    return true;
  }

  bool ComputeShader::uni1fv(const char *varName, GLuint count, float *values)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform1fv(i, count, values);
    return true;
  }

  bool ComputeShader::uni2fv(const char* varName, const float* values)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform2fv(i, 1, values);
    return true;
  }

  bool ComputeShader::uni3fv(const char* varName, const float* values, const int count /*= 0*/)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform3fv(i, count, values);
    return true;
  }

  bool ComputeShader::uni4fv(const char *varName, const float *values)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniform4fv(i, 1, values);
    return true;
  }

  bool ComputeShader::uniMat3(const char *varName, GLfloat *mat, bool transpose)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniformMatrix3fv(i, 1, transpose, mat);
    return true;
  }

  bool ComputeShader::uniMat4(const char *varName, const float*mat, bool transpose /*= false*/)
  {
    GLint i = uniLocation(varName);
    if (i == -1) return false;

    glUniformMatrix4fv(i, 1, transpose, mat);
    return true;
  }

  void ComputeShader::Bind()
  {
    glUseProgram(shaderProgram);
  }

  void ComputeShader::unBind()
  {
    glUseProgram(0);
  }

  GLuint ComputeShader::Get_ID()
  {
    return shaderProgram;
  }
}


