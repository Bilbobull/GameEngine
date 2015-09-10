#include "GraphicsSystem.h"
#include "Buffer_Manager.h"
#include "LoadShader.h"

GraphicsSystem* g_GraphicsSys;
GLuint SimpleProgram;


void GraphicsSystem::Specify_Attributes_Simple(GLuint simpleProgram)
{
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));
}

void GraphicsSystem::Init(void)
{
  Current_Window.glfw_Init();
  g_GraphicsSys = this;

  glEnable(GL_DEPTH_TEST);

  GLfloat vertices[] = {
   -1.0f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f, //0
    0.0f,-1.0f, 0.5f, 0.0f, 1.0f, 0.0f, //1
    1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f, //2

   -1.0f,-1.0f,-0.5f, 1.0f, 0.0f, 0.0f, //3
    1.0f,-1.0f,-0.5f, 1.0f, 0.0f, 0.0f, //4
    0.0f, 1.0f,-0.5f, 0.0f, 1.0f, 0.0f, //5
  };

  GLuint indices[] = { 0, 1, 2, 3, 4, 5 };

  SimpleProgram = LoadShaders("SimpleVertexShader.glsl", "SimpleFragmentShader.glsl");
  VBO* VertexBufferID = new VBO(sizeof(vertices), vertices);
  EBO* IndexBufferID = new EBO(sizeof(indices), indices);

  Specify_Attributes_Simple(SimpleProgram);  
}

void GraphicsSystem::Update(double dt)
{
  glClear(GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, Current_Window.GetWidth(), Current_Window.GetHeight()); // Still need to 
 // glDrawArrays(GL_TRIANGLES, 0, 6);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 // SetBackgroundColor(0.5f, 0.1f, 0.1f);
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


