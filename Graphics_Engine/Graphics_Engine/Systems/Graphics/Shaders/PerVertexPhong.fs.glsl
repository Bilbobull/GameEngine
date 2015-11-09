#version 330

in vec3 Normal;
in vec4 Color;
in vec2 Texcoord;
in vec2 text;

uniform mat4 ModelToWorldMatrix;
uniform mat4 WorldToViewMatrix;
uniform mat4 ViewToProjectionMatrix;

uniform sampler2D Texture;
uniform sampler2D normalTexture;

uniform int Textures;
uniform int NormOrDiff;



uniform int CubeOrNot;


layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outColorNormal;



void main()
{
  if (Textures == 1)
  {
    if (NormOrDiff == 0)
    {
      if (CubeOrNot == 1)
        outColor = texture2D (Texture, text) * Color;
      else
        outColor = texture2D (Texture, Texcoord) * Color;
    }
    else
      outColor = texture2D (normalTexture, text) * Color;
  }

  else
    outColor = Color;
}