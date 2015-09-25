#include "Graphics_Headers.h"
#include "Math_Headers.h"
#include "Buffer_Manager.h"


struct Vertex
{
  glm::vec3 position;
  //glm::vec3 normal;
  glm::vec3 color;
};

class Shape
{
public:
  Shape(){}
  ~Shape(){}

  std::vector <Vertex> vertices;
  std::vector <GLushort> indices;

  VAO* vao;
  VBO* vbo;
  EBO* ebo;
};

class ShapeGenerator 
{
public:
  static Shape makeCube(void);
  static Shape makeArrow(void);
  static Shape makePlane(unsigned dimensions = 10);
  static void SaveVertices(Shape& shape, unsigned dimensions);
  static void SaveIndices(Shape& shape, unsigned dimensions);
  
private:
  
};