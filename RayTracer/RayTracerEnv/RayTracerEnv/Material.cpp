#include "Material.h"

Material::Material()
{
	matColor = Color(0.5,0.5,0.5,1.0);
  Ka = Color(0.1,0.1,0.1);
  Kd = Color(0.7,0.7,0.7);
  Ks = Color(0.3,0.3,0.3);
  reflectionParameter = 0.2;
  refractionIndex = 1.5;
}

Material::Material(Color c, Color input_Ka, Color input_Kd, Color input_Ks, int inputSpecularPower, float inputReflectionParam, float inputRefractionIndex)
{
	matColor = c;
  Ka = input_Ka;
  Kd = input_Kd;
  Ks = input_Ks;
  specularPower = inputSpecularPower;
  reflectionParameter = inputReflectionParam;
  refractionIndex = inputRefractionIndex; 
}