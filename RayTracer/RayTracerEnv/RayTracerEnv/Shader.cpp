//
//  Shader.cpp
//  RayTracerEnv
//
//  Created by hima on 11/14/14.
//  Copyright (c) 2014 hima. All rights reserved.
//

/*  Includes */
#include "Shader.h"
#include "Gz.h"
#include "Disp.h"
#include "Pixel.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Vector.h"
#include "Camera.h"
#include "Ray.h"
#include "Light.h"
#include <iostream>
#include <string>
#include "Render.h"

/*  MACROS */
//@todo do this from a common place
#define MAX_LIGHTS            10
#define SPECULAR_COEFF        32

#define NOT_USED(x) (x = x)

/* structs */
typedef struct{
  Vector E;
  Color Ka;
  Color Kd;
  Color Ks;
}shader_data_s;

shader_data_s shader_data =
{
  .E  = Vector(0,0,-1),
  .Ka = Color(0.1,0.1,0.1),
  .Kd = Color(0.7,0.7,0.7),
  .Ks = Color(0.3,0.3,0.3),
};
/*  forward declarations */


/*  functions */
void getColor
(
  Light   * lights,
  int       numLights,
  GeomObj * objToRender,
  Color   * returnColor
)
{

  Vector *N         = &objToRender->objNormal;
  Vector L          = lights[0].direction.subVector(objToRender->objIntersection);  //objToRender->objIntersection.subVector();
  Vector              R;
  double              NdotL,NdotE,RdotE;
  Vector              localNormal;
  Color               specColor,diffColor,ambColor;
  int flag = 1;
  //int numlights = 1;
  //double ndotL      = N->dotProduct(L);

  L = L.normalize();
  NdotL = N->dotProduct(L);


  for(int l=0; l <numLights; l++)
  {
    flag = 1;
    NdotL = N->dotProduct(L);
    NdotE = N->dotProduct(shader_data.E);

    if(NdotL > 0 && NdotE > 0)
    {
      localNormal = *N;
      flag        = 0;
    }
    else if (NdotL > 0 && NdotE > 0)
    {
      localNormal = N->negate();
      flag        = 0;
    }
    // R = 2(N.L)N-L
    R.x = (2*NdotL*localNormal.x) - lights[l].direction.x; 
    R.y = (2*NdotL*localNormal.y) - lights[l].direction.y; 
    R.z = (2*NdotL*localNormal.z) - lights[l].direction.z; 
    R = R.normalize();
    RdotE = R.dotProduct(shader_data.E); 
    if(RdotE < 0) RdotE = 0;
    if(RdotE > 1) RdotE = 1;
    if(flag == 0)
    {
      // Diffuse Summation
      diffColor.red   += lights[l].lightColor.red   * NdotL;
      diffColor.green += lights[l].lightColor.green * NdotL;
      diffColor.blue  += lights[l].lightColor.blue  * NdotL;
      // Specular Summation
      specColor.red   += lights[l].lightColor.red   * pow(RdotE,SPECULAR_COEFF);
      specColor.green += lights[l].lightColor.green * pow(RdotE,SPECULAR_COEFF);
      specColor.blue  += lights[l].lightColor.blue  * pow(RdotE,SPECULAR_COEFF);
    }
    else
    {
      diffColor.red += 0;diffColor.green +=0;diffColor.blue += 0;
      specColor.red += 0;specColor.green += 0;specColor.blue += 0;
    }
  }
  diffColor.red   = shader_data.Kd.red   * diffColor.red ;
  diffColor.green = shader_data.Kd.green * diffColor.green;
  diffColor.blue  = shader_data.Kd.blue  * diffColor.blue;

  specColor.red   = shader_data.Ks.red   * specColor.red ;
  specColor.green = shader_data.Ks.green * specColor.green;
  specColor.blue  = shader_data.Ks.blue  * specColor.blue;

  Color color01 = Color(objToRender->material.matColor);
  color01.red   = color01.red    *( diffColor.red   + specColor.red);// R
  color01.green = color01.green  *( diffColor.green + specColor.green);// G
  color01.blue  = color01.blue   *( diffColor.blue  + specColor.blue);// B

  (*returnColor) = color01; 
  return ; 
}
