#include "MeshGenerator.h"
#include "GraphicsSystem.h"
#include "OBJLoader.h"


void MeshGenerator::Specify_Attributes(void)
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (char*)(sizeof(GL_FLOAT) * 3));
}

Mesh* MeshGenerator::makeCube(void)
{
  Mesh* cube = new Mesh();
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

    -1.0f, -1.0f, +1.0f, // 15
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
  int NumVertices = ARRAYSIZE(stackVerts) / 6;
  cube->vertices = std::vector <Vertex> (NumVertices);
  memcpy(cube->vertices.data(), stackVerts, sizeof(stackVerts));

  unsigned short stackIndices[] = {
    0, 1, 2, 0, 2, 3, // Top
    4, 5, 6, 4, 6, 7, // Front
    8, 9, 10, 8, 10, 11, // Right
    12, 13, 14, 12, 14, 15, // Left
    16, 17, 18, 16, 18, 19, // Back
    20, 22, 21, 20, 23, 22, // Bottom
  };

  int NumIndices = ARRAYSIZE(stackIndices);
  cube->indices = std::vector <GLushort> (NumIndices);
  memcpy(cube->indices.data (), stackIndices, sizeof(stackIndices));

  cube->vao = new VAO();
  cube->vbo = new VBO(cube->vertices.size() * sizeof(Vertex), cube->vertices.data());
  cube->ebo = new EBO(cube->indices.size() * sizeof(cube->indices[0]), cube->indices.data());

  Specify_Attributes();


  cube->vao->unBind();

  return cube;
}

Mesh* MeshGenerator::makeArrow(void)
{
  Mesh* arrow = new Mesh();

  GLfloat stackVerts[] = {
//top side of arrow head
    +0.00f, +0.25f, -0.25f, //0
    +1.00f, +0.00f, +0.00f, //Color
    +0.50f, +0.25f, -0.25f, //1
    +1.00f, +0.00f, +0.00f, //Color
    +0.00f, +0.25f, -1.00f, //2
    +1.00f, +0.00f, +0.00f, //Color
    -0.50f, +0.25f, -0.25f, //3
    +1.00f, +0.00f, +0.00f, //Color
//bottom side of arrow head 		 
    +0.00f, -0.25f, -0.25f, //4
    +0.00f, +0.00f, +1.00f, //Color
    +0.50f, -0.25f, -0.25f, //5
    +0.00f, +0.00f, +1.00f, //Color
    +0.00f, -0.25f, -1.00f, //6
    +0.00f, +0.00f, +1.00f, //Color
    -0.50f, -0.25f, -0.25f, //7
    +0.00f, +0.00f, +1.00f, //Color
 //right side of arrow tip
    +0.50f, +0.25f, -0.25f, //8
    +0.60f, +1.00f, +0.00f, //Color
    +0.00f, +0.25f, -1.00f, //9
    +0.60f, +1.00f, +0.00f, //Color
    +0.00f, -0.25f, -1.00f, //10
    +0.60f, +1.00f, +0.00f, //Color
    +0.50f, -0.25f, -0.25f, //11
    +0.60f, +1.00f, +0.00f, //Color
 //left side of arrow tip
    +0.00f, +0.25f, -1.00f, //12
    +0.00f, +1.00f, +0.00f, //Color
    -0.50f, +0.25f, -0.25f, //13
    +0.00f, +1.00f, +0.00f, //Color
    +0.00f, -0.25f, -1.00f, //14
    +0.00f, +1.00f, +0.00f, //Color
    -0.50f, -0.25f, -0.25f, //15
    +0.00f, +1.00f, +0.00f, //Color
 //back side of arrow tip
    -0.50f, +0.25f, -0.25f, //16
    +0.50f, +0.50f, +0.50f, //Color
    +0.50f, +0.25f, -0.25f, //17
    +0.50f, +0.50f, +0.50f, //Color
    -0.50f, -0.25f, -0.25f, //18
    +0.50f, +0.50f, +0.50f, //Color
    +0.50f, -0.25f, -0.25f, //19
    +0.50f, +0.50f, +0.50f, //Color
//top side of arrow back
    +0.25f, +0.25f, -0.25f, //20
    +1.00f, +0.00f, +0.00f, //Color
    +0.25f, +0.25f, +1.00f, //21
    +1.00f, +0.00f, +0.00f, //Color
    -0.25f, +0.25f, +1.00f, //22
    +1.00f, +0.00f, +0.00f, //Color
    -0.25f, +0.25f, -0.25f, //23
    +1.00f, +0.00f, +0.00f, //Color
//bottom side of arrow back
    +0.25f, -0.25f, -0.25f, //24
    +0.00f, +0.00f, +1.00f, //Color
    +0.25f, -0.25f, +1.00f, //25
    +0.00f, +0.00f, +1.00f, //Color
    -0.25f, -0.25f, +1.00f, //26
    +0.00f, +0.00f, +1.00f, //Color
    -0.25f, -0.25f, -0.25f, //27
    +0.00f, +0.00f, +1.00f, //Color
   //right side of arrow bck
    +0.25f, +0.25f, -0.25f, //28
    +0.60f, +1.00f, +0.00f, //Color
    +0.25f, -0.25f, -0.25f, //29
    +0.60f, +1.00f, +0.00f, //Color
    +0.25f, -0.25f, +1.00f, //30
    +0.60f, +1.00f, +0.00f, //Color
    +0.25f, +0.25f, +1.00f, //31
    +0.60f, +1.00f, +0.00f, //Color
 //left side of arrow back
    -0.25f, +0.25f, -0.25f, //32
    +0.00f, +1.00f, +0.00f, //Color
    -0.25f, -0.25f, -0.25f, //33
    +0.00f, +1.00f, +0.00f, //Color
    -0.25f, -0.25f, +1.00f, //34
    +0.00f, +1.00f, +0.00f, //Color
    -0.25f, +0.25f, +1.00f, //35
    +0.00f, +1.00f, +0.00f, //Color
  //back side of arrow back
    -0.25f, +0.25f, +1.00f, //36
    +0.50f, +0.50f, +0.50f, //Color
    +0.25f, +0.25f, +1.00f, //37
    +0.50f, +0.50f, +0.50f, //Color
    -0.25f, -0.25f, +1.00f, //38
    +0.50f, +0.50f, +0.50f, //Color
    +0.25f, -0.25f, +1.00f, //39
    +0.50f, +0.50f, +0.50f, //Color
  };

  arrow->vertices = std::vector <Vertex>(ARRAYSIZE(stackVerts) / 6);
  memcpy(arrow->vertices.data(), stackVerts, sizeof(stackVerts));

  GLushort stackIndices[] =
  {
    0, 1, 2, // top
    0, 2, 3,
    4, 6, 5, //bottom
    4, 7, 6,
    8, 10, 9, //right side of arrow tip
    8, 11, 10,
    12, 15, 13, //left side of arrow tip
    12, 14, 15,
    16, 19, 17, //back side of arrow tip
    16, 18, 19,
    20, 22, 21, //top side of back of arrow
    20, 23, 22,
    24, 25, 26, //bottom side of back of arrow
    24, 26, 27,
    28, 30, 29, //right side of back of arrow
    28, 31, 30,
    32, 33, 34, //left side of back of arrow
    32, 34, 35,
    36, 39, 37, //back side of back of arrow
    36, 38, 39,
  };

  arrow->indices = std::vector <GLushort>(ARRAYSIZE(stackIndices));
  memcpy(arrow->indices.data(), stackIndices, sizeof(stackIndices));

  arrow->vao = new VAO();
  arrow->vbo = new VBO(arrow->vertices.size() * sizeof(Vertex), arrow->vertices.data());
  arrow->ebo = new EBO(arrow->indices.size() * sizeof(arrow->indices[0]), arrow->indices.data());

  Specify_Attributes();

  arrow->vao->unBind();

  return arrow;
}

Mesh* MeshGenerator::makePlane(unsigned dimensions /* = 10 */)
{
  Mesh* plane = new Mesh();

  SaveVertices(*plane, dimensions);
  SaveIndices(*plane, dimensions);

  plane->vao = new VAO();
  plane->vbo = new VBO(plane->vertices.size() * sizeof(Vertex), plane->vertices.data());
  plane->ebo = new EBO(plane->indices.size() * sizeof(GLushort), plane->indices.data());

  Specify_Attributes();

  plane->vao->unBind();

  return plane;
}

void MeshGenerator::SaveVertices(Mesh& shape, unsigned dimensions)
{
  int NumVertices = dimensions * dimensions;
  int half = dimensions / 2;
  shape.vertices = std::vector <Vertex> (NumVertices);
  unsigned pos = 0;
  for (unsigned i = 0; i < dimensions; ++i)
  {
    for (unsigned j = 0; j < dimensions; ++j)
    {
      Vertex v;
      v.position = glm::vec3((float)j - (float)half,
        ((float)i - (float)half),
        0.0f);
      
      v.color = glm::vec3 (g_GraphicsSys->Random_Color());
      shape.vertices[pos++] = v;
    }
  }
  return;
}

void MeshGenerator::SaveIndices(Mesh& shape, unsigned dimensions)
{
  int NumIndices = (dimensions - 1) * (dimensions - 1) * 2 * 3; // 2 triangles per square, 3 indices per triangle
  shape.indices = std::vector <GLushort> (NumIndices);
  unsigned pos = 0;

  for (unsigned row = 0; row < dimensions - 1; ++row)
  {
    for (unsigned column = 0; column < dimensions - 1; ++column)
    {
      shape.indices[pos++] = dimensions * row + column;
      shape.indices[pos++] = dimensions * row + column + dimensions;
      shape.indices[pos++] = dimensions * row + column + dimensions + 1;

      shape.indices[pos++] = dimensions * row + column;
      shape.indices[pos++] = dimensions * row + column + dimensions + 1;
      shape.indices[pos++] = dimensions * row + column + 1;
    }
  }

  assert(pos = shape.indices.size());
  return;
}

Mesh* MeshGenerator::Create_Shape(Meshes meshtype)
{
  Mesh* mesh = new Mesh();
  switch (meshtype)
  {
  case Cube_Mesh:
    mesh = makeCube();
    break;

  case Arrow_Mesh:
    mesh = makeArrow();
    break;

  case Plane_Mesh:
    mesh = makePlane();
    break;
    
  }
  return mesh;
}

Mesh* MeshGenerator::makeMeshFromObj(std::string filename)
{
  Mesh* mesh = new Mesh();
  ObjLoader::loadOBJ(filename.c_str(), mesh);

  mesh->vao = new VAO();
  mesh->vbo = new VBO(mesh->vertices.size() * sizeof (Vertex), mesh->vertices.data());
  mesh->ebo = new EBO(mesh->indices.size() * sizeof(GLushort), mesh->indices.data());

  Specify_Attributes();
  mesh->vao->unBind();
  return mesh;
}




