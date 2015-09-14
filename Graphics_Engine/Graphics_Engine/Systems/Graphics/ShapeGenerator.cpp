#include "ShapeGenerator.h"
#include <string>

Shape ShapeGenerator::makeCube()
{
  Shape cube;
  GLfloat stackVerts[] = {
    -1.0f, +1.0f, +1.0f, // 0
    +1.0f, +0.0f, +0.0f, // Colour

    +1.0f, +1.0f, +1.0f, // 1
    +0.0f, +1.0f, +0.0f, // Colour

    +1.0f, +1.0f, -1.0f, // 2
    +0.0f, +0.0f, +1.0f, // Colour

    -1.0f, +1.0f, -1.0f, // 3
    +1.0f, +1.0f, +1.0f, // Colour
    ///////////////////////////////////

    -1.0f, +1.0f, -1.0f, // 4
    +1.0f, +0.0f, +1.0f, // Colour

    +1.0f, +1.0f, -1.0f, // 5
    +0.0f, +0.5f, +0.2f, // Colour

    +1.0f, -1.0f, -1.0f, // 6
    +0.8f, +0.6f, +0.4f, // Colour

    -1.0f, -1.0f, -1.0f, // 7
    +0.3f, +1.0f, +0.5f, // Colour
    ////////////////////////////////////

    +1.0f, +1.0f, -1.0f, // 8
    +0.2f, +0.5f, +0.2f, // Colour

    +1.0f, +1.0f, +1.0f, // 9
    +0.9f, +0.3f, +0.7f, // Colour

    +1.0f, -1.0f, +1.0f, // 10
    +0.3f, +0.7f, +0.5f, // Colour

    +1.0f, -1.0f, -1.0f, // 11
    +0.5f, +0.7f, +0.5f, // Colour
    ///////////////////////////////////

    -1.0f, +1.0f, +1.0f, // 12
    +0.7f, +0.8f, +0.2f, // Colour

    -1.0f, +1.0f, -1.0f, // 13
    +0.5f, +0.7f, +0.3f, // Colour

    -1.0f, -1.0f, -1.0f, // 14
    +0.4f, +0.7f, +0.7f, // Colour

    -1.0f, -1.0f, -1.0f, // 15
    +0.2f, +0.5f, +1.0f, // Colour
    ////////////////////////////////////

    +1.0f, +1.0f, +1.0f, // 16
    +0.6f, +1.0f, +0.7f, // Colour

    -1.0f, +1.0f, +1.0f, // 17
    +0.6f, +0.4f, +0.8f, // Colour

    -1.0f, -1.0f, +1.0f, // 18
    +0.2f, +0.8f, +0.7f, // Colour

    +1.0f, -1.0f, +1.0f, // 19
    +0.2f, +0.7f, +1.0f, // Colour
    ///////////////////////////////////

    +1.0f, -1.0f, -1.0f, // 20
    +0.8f, +0.3f, +0.7f, // Colour

    -1.0f, -1.0f, -1.0f, // 21
    +0.8f, +0.9f, +0.5f, // Colour

    -1.0f, -1.0f, +1.0f, // 22
    +0.5f, +0.8f, +0.5f, // Colour
    +1.0f, -1.0f, +1.0f, // 23
    +0.9f, +1.0f, +0.2f, // Colour
  };
  cube.NumVertices = ARRAYSIZE(stackVerts);
  cube.vertices = new GLfloat[cube.NumVertices];
  memcpy(cube.vertices, stackVerts, sizeof(stackVerts));

  unsigned short stackIndices[] = {
    0, 1, 2, 0, 2, 3, // Top
    4, 5, 6, 4, 6, 7, // Front
    8, 9, 10, 8, 10, 11, // Right
    12, 13, 14, 12, 14, 15, // Left
    16, 17, 18, 16, 18, 19, // Back
    20, 22, 21, 20, 23, 22, // Bottom
  };

  cube.NumIndices = ARRAYSIZE(stackIndices);
  cube.indices = new GLushort[cube.NumIndices];
  memcpy(cube.indices, stackIndices, sizeof(stackIndices));

  cube.vbo = new VBO(cube.NumVertices * sizeof(cube.vertices[0]), cube.vertices);
  cube.ebo = new EBO(cube.NumIndices * sizeof(cube.indices[0]), cube.indices);

  cube.vbo->unBind();
  cube.ebo->unBind();
  return cube;
}