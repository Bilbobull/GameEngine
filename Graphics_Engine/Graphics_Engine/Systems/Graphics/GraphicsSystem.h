// Author: Gonzalo Rojo

#pragma once

#include "Camera\Context.h"
#include "Graphics_Headers.h"
#include "../Systems.h"

class GraphicsSystem : public System
{
public:
  GraphicsSystem(){}
  ~GraphicsSystem(){}

  void Init(void);
  void Update(double dt);
  void Free(void);

private:

};

extern bool Running;