// Author: Gonzalo Rojo

#pragma once

#include "../Graphics/Graphics_Headers.h"
#include "../Graphics/Camera/Context.h"
#include "../Systems.h"

class InputSystem : public System
{
public:
  InputSystem(){}
  ~InputSystem(){}

  void Init(void){}
  void Update(double dt);
  void Free(void){}

private:

};
