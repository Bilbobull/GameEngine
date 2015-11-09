#version 330

#define pi 3.1415926535897932384626433832795

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
  vec4 emisive;
};

// support UP TO 8 lights
uniform int lightTypes[MaxLights]; 
uniform vec4 lightPositions[MaxLights]; 
uniform vec4 lightDirections[MaxLights]; 
uniform vec4 lightAmbients[MaxLights]; 
uniform vec4 lightDiffuses[MaxLights]; 
uniform vec4 lightSpeculars[MaxLights];

uniform float lightInners[MaxLights]; 
uniform float lightOuters[MaxLights]; 
uniform float lightFalloffs[MaxLights];

uniform int LightCount; 

uniform vec4 globalAmbient;

uniform Material MaterialValues;
uniform mat4 ModelToWorldMatrix;
uniform mat4 WorldToViewMatrix;
uniform mat4 ViewToProjectionMatrix;

uniform vec3 CameraPosition = vec3(0.0);
uniform float Shininess;

uniform float DistanceAttConstants[3];
uniform int DistanceAttBool;

uniform float NearPlane;
uniform float FarPlane;
uniform int AtmosphericAttBool;
uniform vec4 AtmosphericIntensity;

uniform int TextureType;
uniform int NormOrDiff;

uniform int Textures;
uniform int NormalYesorNo;

uniform int CubeOrNot;

uniform sampler2D Texture;
uniform sampler2D normalTexture;


out vec4 Color;
out vec3 Normal;
out vec2 Texcoord;
out vec2 text;
out vec3 Position;

vec4 computeLightingTerm(in int lightIdx, in vec4 worldNormal)
{
  vec4 lightdif = lightDiffuses[lightIdx];
  vec4 lightamb = lightAmbients[lightIdx];
  vec4 lightspe = lightSpeculars[lightIdx];
  vec4 newpos = WorldToViewMatrix * ModelToWorldMatrix * vec4(position, 1.0);
  vec4 newCampos = vec4(0,0,0, 1.0);

  vec4 newlightdir = vec4(0);

   vec4 lightdir;
  if(lightTypes[lightIdx] == 0)
  {
    lightdir = lightDirections[lightIdx];
  	newlightdir = WorldToViewMatrix * lightdir;
  }

  else
  {
    lightdir = lightPositions[lightIdx];
    newlightdir = newpos - WorldToViewMatrix * lightdir;
  }


  // AMBIENT
  vec4 ambient = lightamb * MaterialValues.ambient;
  
  // DIFUSE
  vec4 diffuse = vec4(0); // same as vec4(0, 0, 0, 0), or black
  vec4 lightVec = normalize(newlightdir);
  float diffuseFactor = dot(worldNormal, -lightVec);
  if (diffuseFactor > 0) // is there a diffuse contribution?
  {
    // compute diffuse contribution on the surface
    diffuse = diffuseFactor * lightdif * MaterialValues.diffuse;
  }

  vec4 globalamb = globalAmbient * MaterialValues.ambient;

  // SPECULAR
  vec4 specular = vec4(0); 
  vec4 reflection = reflect(lightVec, worldNormal);
  vec4 view = normalize(newCampos - newpos);
  float specularFactor = dot(reflection, view);
  if (specularFactor > 0) 
  {
    specular = pow(specularFactor, Shininess) * lightspe * MaterialValues.specular;
  }

  float Lightdist = length(newlightdir);

  float c1 = DistanceAttConstants[0];
  float c2 = DistanceAttConstants[1];
  float c3 = DistanceAttConstants[2];
  float Att = min(1/(c1+c2*Lightdist+c3*pow(Lightdist,2.0)), 1.0);


 // if(lightTypes[lightIdx] == 1)
 // {
//	float Alpha = dot(lightDirections[lightIdx], newlightdir);
//	float SpotLight = (cos(Alpha) - cos(lightOuters[lightIdx]))/(cos(lightInners[lightIdx]) - cos(lightOuters[lightIdx]));
 //   return  Att * ambient + Att * SpotLight * (diffuse + specular); // total contribution from this light
 // }

  return  Att * ambient +  Att *(diffuse + specular); // total contribution from this light
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

vec4 computeAtmosphericAttenuation(in vec4 color)
{
  vec4 newpos =  WorldToViewMatrix * ModelToWorldMatrix * vec4(Position, 1.0);
  vec4 newCampos = vec4(0,0,0, 1.0);
  vec4 view = newpos - newCampos;
  float ViewLength = length(view);
  float S = clamp( (FarPlane - ViewLength)/(FarPlane - NearPlane), 0 , 1);


  vec4 FinalColor = S * color + (1 - S) * AtmosphericIntensity;

  return FinalColor;


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

  if(AtmosphericAttBool != 0)
    color = computeAtmosphericAttenuation(color);

  vec4 globalamb = globalAmbient * MaterialValues.ambient;
  color += globalamb + MaterialValues.emisive;


  return color; // contribution from all lights onto surface
}

vec2 computeCilindrical(vec4 pos)
{
  vec2 coords;
  float theta = atan(pos.y, pos.x);
  if (theta < 0.0)
    theta += 2.0 * pi;

  coords.x = theta / (2 * pi);
  coords.y = (pos.z - (-1.0)) / (1 - (-1));

  return coords;
}

vec2 computeSpherical(vec4 pos)
{
  vec2 coords;
  float theta = atan(pos.y, pos.x);
  if (theta < 0.0)
    theta += 2.0 * pi;

  float gamma = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
  float alpha = acos(pos.z / gamma);
  if (alpha < 0.0)
    alpha += 2.0 * pi;

  coords.x = theta / (2 * pi);
  coords.y = alpha / pi;

  return coords;
}

vec2 computeCube(vec4 pos)
{
  vec2 coords;

  vec4 newpos = abs(pos);
  vec4 norm = vec4(0.5) + 0.5 * pos;

  if (newpos.x >= newpos.y && newpos.x >= newpos.z)
  {
    coords.x = norm.z;
    coords.y = norm.y;
  }

  else if (newpos.y >= newpos.x && newpos.y >= newpos.z)
  {
    coords.x = norm.x;
    coords.y = norm.z;
  }

  else if (newpos.z >= newpos.x && newpos.z >= newpos.y)
  {
    coords.x = norm.x;
    coords.y = norm.y;
  }

  return coords;
}

vec2 ComputeTexcoords(vec4 pos)
{
  if (TextureType == 0)
    return computeCilindrical(pos);

  if (TextureType == 1)
    return computeSpherical(pos);

  if (TextureType == 2)
    return computeCube(pos);


}


void main ()
{
  vec4 worldNorm;

  vec4 v = vec4(position, 1.0);
  vec4 pos = normalize(v);
  vec2 coor = ComputeTexcoords(pos);
  if (NormalYesorNo == 0)
  {
    vec3 tempnorm = texture2D (normalTexture, coor).rgb;
    worldNorm = normalize(WorldToViewMatrix * ModelToWorldMatrix * vec4(tempnorm, 0.0));
  }
  else
    worldNorm = normalize(WorldToViewMatrix * ModelToWorldMatrix * vec4(normal, 0.0));


  gl_Position = ViewToProjectionMatrix * WorldToViewMatrix * ModelToWorldMatrix * v;
  Normal = normal;
  text = coor;
  Texcoord = texcoord;
  Color = computeSurfaceColor(worldNorm);

}

