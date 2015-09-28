#pragma once

#include <vector>
#include "Math_Headers.h"
#include "Mesh.h"

namespace ObjLoader
{
  bool loadOBJ(
    const char * path,
    Mesh* mesh
    );

  bool loadAssImp(
    const char * path,
    Mesh* pyro
    );
}