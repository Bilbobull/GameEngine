#include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"

GraphicsSystem* g_GraphicsSys;
GLuint SimpleProgram;


void GraphicsSystem::Specify_Attributes_Simple(GLuint simpleProgram)
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  g_GraphicsSys = this;

  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };

  VBO* SimpleVBO = new VBO(sizeof(vertices), vertices);
  Specify_Attributes_Simple(0);
 // SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
}

void GraphicsSystem::Update(double dt)
{
  glViewport(0, 0, Current_Window.GetWidth(), Current_Window.GetHeight());
  glDrawArrays(GL_TRIANGLES, 0, 3);
 // SetBackgroundColor(0.5f, 0.1f, 0.1f);
 // glUseProgram(SimpleProgram);
  glfwSwapBuffers(Current_Window.glfw_GetWindow());
}

void GraphicsSystem::Free(void)
{
  
}

Window& GraphicsSystem::GetCurrentWindow(void)
{
  return Current_Window;
}

void GraphicsSystem::SetBackgroundColor(float r /*= 1.0f*/, float g /*= 1.0f*/, float b /*= 1.0f*/, float a /*= 1.0f*/)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}


