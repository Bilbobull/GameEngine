#version 330

in vec3 Normal;
in vec4 Color;
in vec2 Texcoord;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outColorNormal;

uniform sampler2D Texture;
uniform sampler2D normalTexture;

void main()
{
	outColor = texture2D (Texture, Texcoord) * Color; 
	outColorNormal = texture2D (Texture, Texcoord) * Color; 
}