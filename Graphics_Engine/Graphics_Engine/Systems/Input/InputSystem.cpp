//Author: Gonzalo Rojo

#include "InputSystem.h"
#include "../Graphics/GraphicsSystem.h"

extern bool Running;
InputSystem* g_InputSys;
bool moving;


void InputSystem::Init(void)
{
  moving = false;
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

    switch (key)
    {
    case GLFW_KEY_ESCAPE:
      g_InputSys->CloseWindow();
      break;

    case GLFW_KEY_W:
      g_GraphicsSys->GetCurrentCamera().MoveForward();
      break;

    case GLFW_KEY_S:
      g_GraphicsSys->GetCurrentCamera().MoveBackWard();
      break;

    case GLFW_KEY_A:
      g_GraphicsSys->GetCurrentCamera().MoveLeft();
      break;

    case GLFW_KEY_D:
      g_GraphicsSys->GetCurrentCamera().MoveRight();
      break;

    case GLFW_KEY_E:
      g_GraphicsSys->GetCurrentCamera().MoveUp();
      break;

    case GLFW_KEY_Q:
      g_GraphicsSys->GetCurrentCamera().MoveDown();
      break;

    case GLFW_KEY_5:
      if (action == GLFW_PRESS)
         g_GraphicsSys->DebugDraw();
      break;

    }
}

void InputSystem::CloseWindow(void)
{
  Running = false;
}

void InputSystem::mouseMoveEvent(GLFWwindow* window, double x, double y)
{
  if (moving)
    g_GraphicsSys->GetCurrentCamera().MouseUpdate(glm::vec2(x, y));
}

void InputSystem::mousePressEvent(GLFWwindow* window, int button, int action, int mode)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    moving = true;

  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    moving = false;
}

