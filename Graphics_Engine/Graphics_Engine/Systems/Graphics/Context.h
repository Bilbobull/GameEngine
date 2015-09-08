// Author : Gonzalo Rojo 

#pragma once

#include "Graphics_Headers.h"
#include <string>

class Window
{
public:
  Window();
  ~Window();

  Window(Window&) = delete;
  void operator= (Window&) = delete;

  void sdl_Init();
  bool sdl_WindowShouldClose();

  void glfw_Init();
  GLFWwindow* glfw_GetWindow();
  static void glfw_WindowShouldClose();

  void SetName(std::string title = "Graphics Engine");

private:
  SDL_Window  *sdl_window;
  int major_;
  int minor_;

  const GLFWvidmode* Desktop;
  GLFWwindow* glfw_window;

  std::string name;
  int Width;
  int Height;
};