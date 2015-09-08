#include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"

GraphicsSystem* g_GraphicsSys;
GLuint SimpleProgram;

void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  g_GraphicsSys = this;

  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };

  VBO(ARRAYSIZE(vertices), vertices);
  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
}

void GraphicsSystem::Update(double dt)
{
  SetBackgroundColor(0.5f, 0.1f, 0.1f);
  glUseProgram(SimpleProgram);
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
