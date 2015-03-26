// Author : Gonzalo Rojo 

#pragma once

#include "../Graphics_Headers.h"

class Window
{
public:
  Window();
  ~Window(){}
  void Init();
  bool WindowShouldClose();

private:
  SDL_Window  *sdl_window;
  int major_;
  int minor_;
};

extern Window Current_Window;