#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 vertexColor;
//layout (location = 2) in mat4 FullTransformMatrix;

uniform mat4 FullTransformMatrix;
 
out vec3 Normal;
out vec4 Color;

void main ()
{
  vec4 v = vec4(position, 1.0);

  gl_Position = FullTransformMatrix * v;
  Normal = normal;
  Color = vec4(vertexColor.rbg, 0.4);
}
