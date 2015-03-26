// Author : Gonzalo Rojo 

#pragma once

#include "../Graphics_Headers.h"
#include <string>

class Window
{
public:
  Window();
  ~Window(){}
  void Init();
  bool WindowShouldClose();
  void SetName(std::string title = "Graphics Engine");

private:
  SDL_Window  *sdl_window;
  std::string name;
  int major_;
  int minor_;
};

extern Window Current_Window;