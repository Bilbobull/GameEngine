//Author: Gonzalo Rojo

#include "InputSystem.h"
#include "../Graphics/GraphicsSystem.h"

extern bool Running;
InputSystem* g_InputSys;


void InputSystem::Init(void)
{
  g_InputSys = this;
  glfwSetKeyCallback(g_GraphicsSys->GetCurrentWindow().glfw_GetWindow(), (GLFWkeyfun)Key_Callback);
}

void InputSystem::Update(double dt)
{
  glfwPollEvents();
}

void InputSystem::Free(void)
{
  g_InputSys = nullptr;
}

void InputSystem::Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    g_InputSys->CloseWindow();
}

void InputSystem::CloseWindow(void)
{
  Running = false;
}

