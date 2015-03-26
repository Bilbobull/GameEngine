// Author : Gonzalo Rojo 

#pragma once

#include "../Graphics_Headers.h"

class Window
{
public:
  Window();
  ~Window(){}
  void Init();

private:
  SDL_Window  *Current_Window;
  int major_;
  int minor_;
};

extern Window window;