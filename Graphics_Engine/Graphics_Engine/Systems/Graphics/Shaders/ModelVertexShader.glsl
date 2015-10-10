#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoord;

const int MaxLights = 8; 

// represents material properties of the surface passed by the application
struct Material
{
  vec4 ambient; // ambient color of the surface/how much ambient light to absorb
  vec4 diffuse; // diffuse color of the surface/how much diffuse light to absorb
  vec4 specular;
};


uniform vec4 lightDirections[MaxLights]; // support UP TO 8 lights
uniform vec4 lightAmbients[MaxLights]; // support UP TO 8 lights
uniform vec4 lightDiffuses[MaxLights]; // support UP TO 8 lights
uniform vec4 lightSpeculars[MaxLights]; // support UP TO 8 lights

uniform int LightCount; 
uniform Material MaterialValues;
uniform mat4 ModelToWorldMatrix;
uniform mat4 WorldToViewMatrix;
uniform mat4 ViewToProjectionMatrix;

uniform vec3 CameraPosition = vec3(0.0);
uniform int Shininess;

out vec4 Color;
out vec3 Normal;
out vec2 Texcoord;


vec4 computeLightingTerm(in int lightIdx, in vec4 worldNormal)
{
  vec4 lightdir = lightDirections[lightIdx];
  vec4 lightdif = lightDiffuses[lightIdx];
  vec4 lightamb = lightAmbients[lightIdx];
  vec4 lightspe = lightSpeculars[lightIdx];
  vec4 newpos = WorldToViewMatrix * ModelToWorldMatrix * vec4(position, 1.0);
  vec4 newCampos = vec4(0,0,0, 1.0);
  vec4 newlightdir = WorldToViewMatrix * lightdir;

  // AMBIENT
  vec4 ambient = lightamb * MaterialValues.ambient;
  
  // DIFUSE
  vec4 diffuse = vec4(0); // same as vec4(0, 0, 0, 0), or black
  vec4 lightVec = normalize(-newlightdir);
  float diffuseFactor = dot(worldNormal, lightVec);
  if (diffuseFactor > 0) // is there a diffuse contribution?
  {
    // compute diffuse contribution on the surface
    diffuse = diffuseFactor * lightdif * MaterialValues.diffuse;
  }

  // SPECULAR
  vec4 specular = vec4(0); 
  vec4 reflection = reflect(-lightVec, worldNormal);
  vec4 view = normalize(newCampos - newpos);
  float specularFactor = dot(reflection, view);
  if (specularFactor > 0) 
  {
    specular = pow(specularFactor, Shininess) * lightspe * MaterialValues.specular;
  }

  return ambient + diffuse + specular; // total contribution from this light
}

vec4 computeSurfaceColor(in vec4 worldNormal)
{
  // Phong: total contribution of light is sum of all individual light contribs.
  vec4 color = vec4(0, 0, 0, 0); // no light = black
  for (int i = 0; i < LightCount; ++i)
    color += computeLightingTerm(i, worldNormal); // contribution of light i
  return color; // contribution from all lights onto surface
}

void main ()
{
  vec4 v = vec4(position, 1.0);

  gl_Position = ViewToProjectionMatrix * WorldToViewMatrix * ModelToWorldMatrix * v;
  Normal = normal;
  vec4 worldNorm = normalize( WorldToViewMatrix * ModelToWorldMatrix * vec4(normal, 0.0));
  Color = computeSurfaceColor(worldNorm);
  Texcoord = texcoord;
}

