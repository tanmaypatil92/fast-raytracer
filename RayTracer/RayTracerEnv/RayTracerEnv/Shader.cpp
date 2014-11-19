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
#include "string.h"

/*  MACROS */
//@todo do this from a common place
#define MAX_LIGHTS            10
#define SPECULAR_COEFF        32
#define TRACER_THRESH   0.0000001
#define INFINITY std::numeric_limits<double>::max()

/*  MACROS */
#define _USE_MATH_DEFINES
#define NOT_USED(x) (x = x)

/* structs */
typedef struct{
  Vector E;
  Color Ka;
  Color Kd;
  Color Ks;
}shader_data_s;

static shader_data_s shader_data;

/*
shader_data_s shader_data =
{
  .E  = Vector(0,0,-1),
  .Ka = Color(0.1,0.1,0.1),
  .Kd = Color(0.7,0.7,0.7),
  .Ks = Color(0.3,0.3,0.3),
};
*/

/*  forward declarations */
void init_shader()
{
	//shader_data
  shader_data.E  = Vector(0,0,-1);
  shader_data.Ka = Color(0.1,0.1,0.1);
  shader_data.Kd = Color(0.7,0.7,0.7);
  shader_data.Ks = Color(0.3,0.3,0.3);
}

/*  functions */

void getColor
(
  Light   * lights,
  int       numLights,
  GeomObj * objToRender,
  Color   * returnColor,
  GeomObj * objsToRender[],
  Ray * tempRay,
  int numObjects
)
{

	Vector *N         = &objToRender->objNormal;
	Vector L             ;  //objToRender->objIntersection.subVector();
	Vector              R;
	double              NdotL,NdotE,RdotE;
	Vector              localNormal;
	Color               specColor,diffColor,ambColor;
	int flag = 1;
  
	//L = L.normalize();
	//NdotL = N->dotProduct(L);

	memset(&specColor, 0x0, sizeof(specColor));
	memset(&diffColor, 0x0, sizeof(diffColor));
	memset(&ambColor, 0x0, sizeof(ambColor));
	ambColor.red = 0.1;
	ambColor.green = 0.1;
	ambColor.blue = 0.1;

	Color color01 = Color(objToRender->material.matColor);
	color01.red   = color01.red   + (shader_data.Ka.red   *ambColor.red);
	color01.green = color01.green + (shader_data.Ka.green *ambColor.green);
	color01.blue  = color01.blue  + (shader_data.Ka.blue  *ambColor.blue);
	Vector currectObjectIntersection = objToRender->objIntersection.normalize();
	Vector currentObjectNormal = objToRender->objNormal.normalize();
	double currentObjInterValue = objToRender->intersectionValue;
	bool shadow = true;

	Ray specularRay;
	specularRay.origin    = currectObjectIntersection.subVector(tempRay->origin);
	 // 2(N.L)*N - L where N is normal and L is negative ray direction
	double dotProd = (currentObjectNormal.dotProduct(tempRay->direction.negate()));
	specularRay.direction = (currentObjectNormal.scalarMult(dotProd*2)).subVector(tempRay->direction.negate());
	specularRay.direction = specularRay.direction.normalize();

	for(int l=0; l <numLights; l++)
	{
		L = lights[l].direction.normalize();//subVector(currectObjectIntersection).normalize();
		double distance = L.lengthVector();
		NdotL = objToRender->objNormal.dotProduct(L);
		if(NdotL > 0)
		{
			shadow = false;
			Ray shadowRay = Ray (currectObjectIntersection,L);

			int geomType = objToRender->geometryType;
			/*  Now loop through all the objects */
			for(int obj = 0;obj<numObjects;obj++)
			{
			//Perform the ray intersection with this object
				objsToRender[obj]->findIntersection(shadowRay);
				if(objsToRender[obj]->intersectionValue > TRACER_THRESH && objsToRender[obj]->intersectionValue <= distance)
				{
					if(objsToRender[obj]->geometryType!=geomType)
						shadow = true;
				}
			} /* End of obj loop */

			if(shadow == false)
			{
				color01.red   = color01.red   + (shader_data.Kd.red   * lights[l].lightColor.red   * NdotL);
				color01.green = color01.green + (shader_data.Kd.green * lights[l].lightColor.green * NdotL);
				color01.blue  = color01.blue  + (shader_data.Kd.blue  * lights[l].lightColor.blue  * NdotL);
			}

			double RdotL = specularRay.direction.dotProduct(L);
			double spec;
			if(RdotL > 0)
			{
				spec = pow(RdotL,25);
				color01.red   = color01.red   + (shader_data.Ks.red   * lights[l].lightColor.red   * spec);
				color01.green = color01.green + (shader_data.Ks.green * lights[l].lightColor.green * spec);
				color01.blue  = color01.blue  + (shader_data.Ks.blue  * lights[l].lightColor.blue  * spec);
			}
	   }
		
	}
    
    (*returnColor) = color01; 

	#if 0
	  for(int l=0; l <numLights; l++)
	  {
		L          = lights[l].direction.subVector(objToRender->objIntersection);
		L = L.normalize();
		NdotL = N->dotProduct(L);

		flag = 1;
		NdotE = N->dotProduct(shader_data.E);

		if(NdotL > 0 && NdotE > 0)
		{
		  localNormal = *N;
		  flag        = 0;
		}
		else if (NdotL < 0 && NdotE < 0)
		{
		  localNormal = N->negate();
		  NdotL       = -1.0 * NdotL;
		  flag        = 0;
		}
		// R = 2(N.L)N-L
		R.x = (2*NdotL*localNormal.x) - L.x;
		R.y = (2*NdotL*localNormal.y) - L.y;
		R.z = (2*NdotL*localNormal.z) - L.z;
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
	  }
	  diffColor.red   = shader_data.Kd.red   * diffColor.red ;
	  diffColor.green = shader_data.Kd.green * diffColor.green;
	  diffColor.blue  = shader_data.Kd.blue  * diffColor.blue;

	  specColor.red   = shader_data.Ks.red   * specColor.red ;
	  specColor.green = shader_data.Ks.green * specColor.green;
	  specColor.blue  = shader_data.Ks.blue  * specColor.blue;
  
	  ambColor.red = shader_data.Ka.red * ambColor.red;
	  ambColor.green = shader_data.Ka.green * ambColor.green;
	  ambColor.blue = shader_data.Ka.blue * ambColor.blue;

	  Color color01 = Color(objToRender->material.matColor);
	   // Color should be multiplied?? or remove color component
	  color01.red   += diffColor.red   + specColor.red   + ambColor.red;// R
	  color01.green += diffColor.green + specColor.green + ambColor.green;// G
	  color01.blue  += diffColor.blue  + specColor.blue  + ambColor.blue;// B

	
	  (*returnColor) = color01; 
	#endif
}
