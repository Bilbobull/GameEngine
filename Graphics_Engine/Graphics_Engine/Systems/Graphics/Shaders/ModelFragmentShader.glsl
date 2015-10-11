#version 330

in vec3 Normal;
in vec3 Color;
in vec2 Texcoord;
in vec3 Position;

const int MaxLights = 8; 

// represents material properties of the surface passed by the application
struct Material
{
  vec4 ambient; // ambient color of the surface/how much ambient light to absorb
  vec4 diffuse; // diffuse color of the surface/how much diffuse light to absorb
  vec4 specular;
};

uniform int lightTypes[MaxLights]; // support UP TO 8 lights
uniform vec4 lightPositions[MaxLights]; // support UP TO 8 lights
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
uniform float Shininess;

uniform float DistanceAttConstants[3];
uniform int DistanceAttBool;

uniform sampler2D Texture;
uniform sampler2D normalTexture;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outColorNormal;


vec4 computeLightingTerm(in int lightIdx, in vec4 worldNormal)
{
  vec4 lightdir;
  if(lightTypes[lightIdx] == 0)
    lightdir = lightDirections[lightIdx];
  else
    lightdir = lightPositions[lightIdx];

  vec4 lightdif = lightDiffuses[lightIdx];
  vec4 lightamb = lightAmbients[lightIdx];
  vec4 lightspe = lightSpeculars[lightIdx];
  vec4 newpos = WorldToViewMatrix * ModelToWorldMatrix * vec4(Position, 1.0);
  vec4 newCampos = vec4(0,0,0, 1.0);

  vec4 newlightdir = vec4(0);
  if(lightTypes[lightIdx] == 0)
	newlightdir = WorldToViewMatrix * lightdir;
  else
    newlightdir = newpos - WorldToViewMatrix * lightdir;

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

float computeDistanceAttenuation(in int lightIdx)
{
  float c1 = DistanceAttConstants[0];
  float c2 = DistanceAttConstants[1];
  float c3 = DistanceAttConstants[2];
  vec4 newpos = WorldToViewMatrix * ModelToWorldMatrix * vec4(Position, 1.0);
  vec4 lightvec;
  if(lightTypes[lightIdx] == 0)
  {
    lightvec = lightDirections[lightIdx];
	lightvec = WorldToViewMatrix * lightvec;
  }
  else
  {
    lightvec = lightPositions[lightIdx];
	lightvec = newpos - WorldToViewMatrix * lightvec;
  }
  float Lightdist = length(lightvec);

  float Att = min(1/(c1+c2*Lightdist+c3*pow(Lightdist,2.0)), 1.0);

  return Att;

}

vec4 computeSurfaceColor(in vec4 worldNormal)
{
  // Phong: total contribution of light is sum of all individual light contribs.
  vec4 color = vec4(0, 0, 0, 0); // no light = black
  for (int i = 0; i < LightCount; ++i)
    color += computeLightingTerm(i, worldNormal); // contribution of light i

  if(DistanceAttBool != 0)
  {
    for (int i = 0; i < LightCount; ++i)
      color *= computeDistanceAttenuation(i);
  }

  return color; // contribution from all lights onto surface
}

void main()
{
    vec4 worldNorm = normalize( WorldToViewMatrix * ModelToWorldMatrix * vec4(Normal, 0.0));
	outColor = texture2D (Texture, Texcoord) * computeSurfaceColor(worldNorm); 
	outColorNormal = texture2D (Texture, Texcoord) * vec4(Color, 1.0); 
}