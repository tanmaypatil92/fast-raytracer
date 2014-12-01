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
#define PI 3.14

#ifndef OS_X_ENV
#define INFINITY std::numeric_limits<double>::max()
#endif

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

//Color *image;
//int xs, ys;

int getIndex(int x, int y, int xs) {
	return x + (y * xs);
}

int addGaussianTexture(float u, float v, Color *color)  {
	float row = 0.8, stdU = 1, stdV = 1, muU = 0.5, muV = 0.5;
	float fXY, tmpVar1;
	float pi = 3.14159265358979323846;

	tmpVar1 = (1/(2*pi*stdU*stdV*(sqrt(1 - pow(row, 2)))));
	fXY = (tmpVar1) * exp((-0.5/(1 - pow(row, 2)) * ( ( (pow((u - muU), 2))/pow(stdU, 2) ) + ( (pow((v - muV), 2))/pow(stdV, 2) ) - ( (2*row*(u-muU)*(v-muV))/(stdU*stdV)) )));

	int indicator = floor(fXY * 100);
	indicator = indicator % 2;
	if (indicator == 0)	{
    color->red = 1; 
    color->green = 0; 
    color->blue = 0;
	}
	else	{
		color->red = 1; 
		color->green = 1; 
		color->blue = 1; 
  }

  return 1;
}
int addTexture(float u, float v, Color *color, char *fileName, int reset, Color **image, int *xs, int *ys)
{

	unsigned char		pixel1[3];
	unsigned char     dummy;
	char  		foo[8];
	int   		i;//, j;
	FILE			*fd;

	if (reset) {          /* open and load texture file */
		fd = fopen (fileName, "rb");
		if (fd == NULL) {
			fprintf (stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf (fd, "%s %d %d %c", foo, xs, ys, &dummy);
		*image = (Color*)malloc(sizeof(Color)*(*xs+1)*(*ys+1));
		//Color *image2 = (Color *)malloc(sizeof(Color)*(xs+1)*(ys+1));
		if (*image == NULL) {
			fprintf (stderr, "malloc for texture image failed\n");
			exit(-1);
		}

		for (i = 0; i < (*xs)*(*ys); i++) {	/* create array of GzColor values */
			fread(pixel1, sizeof(pixel1), 1, fd);
			//std::cout<<(float)((int)pixel1[0]) * (1.0 / 255.0);
			(*image)[i].red = (float)((int)pixel1[0]) * (1.0 / 255.0);
			(*image)[i].green = (float)((int)pixel1[1]) * (1.0 / 255.0);
			(*image)[i].blue = (float)((int)pixel1[2]) * (1.0 / 255.0);
		}

		reset = 0;          /* init is done */
		fclose(fd);
	}

	float s, t;
	s = u < 0? 0 : u;
	t = v < 0? 0 : v;
	s = u > 1? 1 : u;
	t = v > 1? 1 : v;

	float x = s * (*xs - 1);
	float y = t * (*ys - 1);

	int A = getIndex(ceil(x), ceil(y), *xs);
	int B = getIndex(floor(x), ceil(y), *xs);
	int C = getIndex(floor(x), floor(y), *xs);
	int D = getIndex(ceil(x), floor(y), *xs);

	// Interpolate
	color->red += s*t*(*image)[C].red + (1-s)*t*(*image)[D].red+ s*(1-t)*(*image)[B].red + (1-s)*(1-t)*(*image)[A].red;
	color->green += s*t*(*image)[C].green + (1-s)*t*(*image)[D].green + s*(1-t)*(*image)[B].green + (1-s)*(1-t)*(*image)[A].green;
	color->blue += s*t*(*image)[C].blue + (1-s)*t*(*image)[D].blue + s*(1-t)*(*image)[B].blue + (1-s)*(1-t)*(*image)[A].blue;

	return 1;
}

/*  functions */

void getColor
(
  Light   * lights,
  int       numLights,
  GeomObj * currentObject,
  Color   * currentColor,
  GeomObj * objsToRender[],
  Ray * incidentRay,
  int numObjects
)
{
	Vector L, R;
	double NdotL;
	Vector localNormal, currectObjectIntersection, currentObjectNormal;
  double currentObjInterValue;
	Color  specColor, diffColor, ambColor;
	//int flag = 1;
  bool shadow = true;
	Ray specularRay;

	ambColor.red = 0.1;
	ambColor.green = 0.1;
	ambColor.blue = 0.1;

  

  /*
	currectObjectIntersection = currentObject->objIntersection.normalize();
	currentObjectNormal = currentObject->objNormal.normalize();
	currentObjInterValue = currentObject->intersectionValue;

	specularRay.origin    = currectObjectIntersection.subVector(incidentRay->origin);
	 // 2(N.L)*N - L where N is normal and L is negative ray direction
	double dotProd = (currentObjectNormal.dotProduct(incidentRay->direction.negate()));
	specularRay.direction = (currentObjectNormal.scalarMult(dotProd*2)).subVector(incidentRay->direction.negate());
	specularRay.direction = specularRay.direction.normalize();
  */

    if(currentObject->objectType == 1){
		Sphere *sph = (Sphere *)currentObject;
		Vector p = currentObject->objIntersection.subVector(sph->center);
		p = p.normalize();

		(*currentColor).red   =  (currentObject->material.Ka.red * ambColor.red);
		(*currentColor).green =  (currentObject->material.Ka.green * ambColor.green);
		(*currentColor).blue  =  (currentObject->material.Ka.blue * ambColor.blue);

		float u = ((atan2(p.x, p.z) / PI) + 1.0f) * 0.5f;
		float v = (asin(p.y) / PI) + 0.5f;
		//addTexture(u,v,currentColor,sph->mat.fileName, sph->mat.reset, &(sph->image), &(sph->xs), &(sph->ys));
		//#ifndef ProceduralTexture
		if (sph->mat.textureType == 2)  {
 	  		addTexture(u,v,currentColor,sph->mat.fileName, sph->mat.reset, &(sph->image), &(sph->xs), &(sph->ys));
		}
		else  {
			if (sph->mat.procTextureType == 1)
				addGaussianTexture(u,v,currentColor);
		}
		sph->mat.reset = 0;
		
	}else{
		(*currentColor) = currentObject->material.matColor;

		(*currentColor).red   +=  (currentObject->material.Ka.red * ambColor.red);
		(*currentColor).green +=  (currentObject->material.Ka.green * ambColor.green);
		(*currentColor).blue  +=  (currentObject->material.Ka.blue * ambColor.blue);
	}

	currectObjectIntersection = currentObject->objIntersection;
	currentObjectNormal = currentObject->objNormal.normalize();
	currentObjInterValue = currentObject->intersectionValue;



  // 2(N.L)*N - L where N is normal and L is negative ray direction
  // Equations for the specular ray directly follow from class notes.
	specularRay.origin    = currectObjectIntersection; //.subVector(incidentRay->origin);
	double dotProd = (currentObjectNormal.dotProduct(incidentRay->direction.negate()));
	specularRay.direction = (currentObjectNormal.scalarMult(dotProd*2)).subVector(incidentRay->direction.negate());
	specularRay.direction = specularRay.direction.normalize();

	for(int l=0; l <numLights; l++)
	{
		L = lights[l].direction.normalize();
		double distance = INFINITY;
		NdotL = currentObject->objNormal.dotProduct(L);
		if(NdotL > 0)
		{
			shadow = false;
			Ray shadowRay = Ray(currectObjectIntersection,L);

			int currentObjId = currentObject->objectId;
			/*  Now loop through all the objects */
			for(int obj = 0;obj<numObjects;obj++)
			{

  /*
			//Perform ray intersection with this object
				objsToRender[obj]->findIntersection(shadowRay);
				if(objsToRender[obj]->intersectionValue > TRACER_THRESH && objsToRender[obj]->intersectionValue <= currentIntersectionValue)
				{
          currentIntersectionValue = objsToRender[obj]->intersectionValue;
					//if(objsToRender[obj]->objectId!=currentObjId)
					shadow = true;
    */

			//Perform the ray intersection with this object
				if(objsToRender[obj]->objectId!=currentObjId)
				{
					objsToRender[obj]->findIntersection(shadowRay, 0);
					if(objsToRender[obj]->intersectionValue > TRACER_THRESH && objsToRender[obj]->intersectionValue <= distance)
					{
              distance = objsToRender[obj]->intersectionValue;
							shadow = true;
					}
				}
			} /* End of obj loop */

			if(shadow == false)
			{
				(*currentColor).red   += (currentObject->material.Kd.red   * lights[l].lightColor.red   * NdotL);
				(*currentColor).green += (currentObject->material.Kd.green * lights[l].lightColor.green * NdotL);
				(*currentColor).blue  += (currentObject->material.Kd.blue  * lights[l].lightColor.blue  * NdotL);

				// Shouldn't render specular if object in shadow I assume
				double RdotL = specularRay.direction.dotProduct(L);
				double spec;
				if(RdotL > 0)
				{
					spec = pow(RdotL,currentObject->material.specularPower);
					(*currentColor).red   += (currentObject->material.Ks.red   * lights[l].lightColor.red   * spec);
					(*currentColor).green += (currentObject->material.Ks.green * lights[l].lightColor.green * spec);
					(*currentColor).blue  += (currentObject->material.Ks.blue  * lights[l].lightColor.blue  * spec);
				}
			}
	   }
    //(*currentColor).colorCheck();
	}

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
