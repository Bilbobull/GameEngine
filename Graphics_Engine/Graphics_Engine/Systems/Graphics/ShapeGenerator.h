#include "Graphics_Headers.h"
#include "Buffer_Manager.h"


class Shape
{
public:
  Shape(){}
  ~Shape(){}

  int NumVertices;
  GLfloat* vertices;
  int NumIndices;
  GLushort* indices;

  VBO* vbo;
  EBO* ebo;
};

class ShapeGenerator
{
public:
  static Shape makeCube(void);
private:
  
};