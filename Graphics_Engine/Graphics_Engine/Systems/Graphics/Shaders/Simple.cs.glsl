#version 430

layout(std140, binding = 0) buffer Pos
{
  vec4 Positions[];
};