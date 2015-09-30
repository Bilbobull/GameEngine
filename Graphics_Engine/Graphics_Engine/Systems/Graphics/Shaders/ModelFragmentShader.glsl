#version 330

in vec3 Normal;
out vec4 outColor;
in vec4 Color;
in vec2 Texcoord;

uniform sampler2D Texture;

void main()
{
	outColor = texture2D (Texture, Texcoord) * Color;
}