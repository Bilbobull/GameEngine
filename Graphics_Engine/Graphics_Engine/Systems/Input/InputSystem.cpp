//Author: Gonzalo Rojo

#include "InputSystem.h"

extern bool Running;

void InputSystem::Update(double dt)
{
  glfwPollEvents();

  if (glfwGetKey(Current_Window.glfw_GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    Running = false;
  }
}

