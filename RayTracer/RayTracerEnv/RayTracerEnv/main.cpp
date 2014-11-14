//
//  main.cpp
//  RayTracerEnv
//
//  Created by hima on 11/11/14.
//  Copyright (c) 2014 hima. All rights reserved.
//  @original code from uthara (thelagar@usc.edu)

/* Raytracer Framework for CS580 Project*/

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


#define MAX_LIGHTS 10
#define MAX_SPHERES 10
#define MAX_PLANES 1
#define MAX_OBJ_TO_RENDER 5

typedef struct
{

  int dispWidth;
  int dispHeight;
  
  float defaultCamFOV;
  
  Vector camPosition;
  Vector camLook;
  //Camera defCam;
  
  /* The actual number of lights to initialize*/
  int numLights;
  Color defLightColors[MAX_LIGHTS];
  
  
  int numSpheres;
  Sphere spheres[MAX_SPHERES];
  
  int numPlanes;
  Plane planes[MAX_PLANES];
  
} infinity_bsp_s;

/* Stores all the elemental structures needed for initializing the app data */
infinity_bsp_s infinity_bsp =
{
  .dispWidth = XRES,
  .dispHeight = YRES,
  
  .defaultCamFOV = 45,
  
  .camPosition = Vector(0,-2.5,-10),
  .camLook      = Vector(0,0,3.0),
  
  .numLights = 1,
  .defLightColors = {Color(1.0,1.0,1.0), Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0),Color(1.0,1.0,1.0)},
  
  
  .numSpheres = 2,
  .spheres = {Sphere(Vector(0.0,1.2,0.0),1.0,Material(Color(0.4,0.1,0.0,1.0))), Sphere(Vector(3.0,1.2,0.0),1.0,Material(Color(0.4,0.1,0.0,1.0)))},

  .numPlanes = 1,
  .planes = {}
  
};

typedef struct
{

  Camera appCamera;
  
  GeomObj* objectsToRender[MAX_OBJ_TO_RENDER];

}infinity_tracer_app_data_s;

infinity_tracer_app_data_s tracer_app = {

  .appCamera = Camera(infinity_bsp.camPosition,infinity_bsp.camLook),
  
  .objectsToRender = {NULL},
  
  
};
//Next step is checkerboard...
// create 10^-6 check for intersection value greater than 0.000001
// for reflection check on : http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-1-writing-a-simple-raytracer/

#include "math.h"

int main(int argc,char **argv)
{
  GeomObj * thisObj1 = NULL;
  GeomObj * thisObj2 = NULL;
  
	int width  = XRES;
	int height = YRES;
	//double ARatio = (double)width/(double)height;
	int result = 0;
	float FOV = 45;
	float PI = 3.14159265;
	//Camera information
	Vector camPosition = Vector(0,-2.5,10);
	Vector camLook     = Vector(0,0.0,3.0);
	Camera cam01       = Camera(camPosition,camLook);

	//Light Information
	Color  lightColor     = Color(1.0,1.0,1.0);
	//Vector lightDirection = Vector(-30,-10,20);
	Vector lightDirection = Vector(0,0.0,-10.0);
	Light  light01        = Light(lightDirection,lightColor);

	//Sphere
	Material sphereMat  = Material(Color(0.4,0.1,0.0,1.0));
	double sphereRadius = 1.0;
	Vector sphereCenter = Vector(0.0,1.2,0.0); // World space coordinates
	Sphere sphere01 = Sphere(sphereCenter,sphereRadius,sphereMat);
	
  	//Sphere
	Material sphereMat2  = Material(Color(0.4,0.1,0.0,1.0));
	double sphereRadius2 = 1.0;
	Vector sphereCenter2 = Vector(3.0,1.2,0.0); // World space coordinates
	Sphere sphere02 = Sphere(sphereCenter2,sphereRadius2,sphereMat2);
  
  
  double intersectionValue,intersectionValue2;
	Vector tempdirection;


	Pixel *pixels = (Pixel*)malloc(sizeof(Pixel)*width*height);
	
  std::cout<<"Begin rendering ..."<<std::endl;

  float theta = tanf((PI*(FOV/2.0))/180);
  float ratio = (float)height/(float)width;
  float halfheight = theta*ratio;
  float cameraWidth = theta*2.0;
  float cameraHeight = halfheight*2.0;
  float pixelWidth = cameraWidth/(width-1);
  float pixelHeight = cameraHeight/(height-1);
  
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{
			// transforming x and y for ray calculation from screen to camera space


      float xScaleCoeff = (x*pixelWidth)-theta ;
      float yScaleCoeff = (y*pixelHeight)-halfheight;
    

      if( (xScaleCoeff < -1.0) || (xScaleCoeff > 1.0))
      {
        std::cout<<"xoffset not normalized "<<xScaleCoeff<<std::endl;
      }
      if( (yScaleCoeff < -1.0) || (yScaleCoeff > 1.0))
      {
        std::cout<<"yoffset not normalized "<<yScaleCoeff<<std::endl;
      }
    
      Vector xoffset = cam01.camX.scalarMult(xScaleCoeff);
			Vector yoffset = cam01.camY.scalarMult(yScaleCoeff);
			
			// Calculating camera ray using the new x and y offsets
			Ray cameraRay;
			cameraRay.origin= cam01.position;
			cameraRay.direction = (cam01.camZ.addVector(xoffset)).addVector(yoffset);
			cameraRay.direction = cameraRay.direction.normalize();
	
			//intersectionValue = plane01.planeIntersection(cameraRay);
			intersectionValue  = sphere01.sphereIntersection(cameraRay);
			Vector normalValue = sphere01.sphereNormal;
			Vector interValue  = sphere01.sphereInter;

			intersectionValue2  = sphere02.sphereIntersection(cameraRay);
			Vector normalValue2 = sphere02.sphereNormal;
			Vector interValue2  = sphere02.sphereInter;
    
			//Vector lightOffset = light01.direction.subVector(interValue);
			//double lightDistance = lightOffset.lengthVector();
			//Vector lightDirection = lightOffset.normalize();
			//double dotProduct = normalValue.dotProduct(lightDirection);

			//Vector N = normalValue;
			//Vector L = (light01.direction).subVector(interValue);
			//double DOT = N.dotProduct(L);

			if(intersectionValue > 1 || intersectionValue2 > 1)
			{
        double intersectionVal = intersectionValue2;
        Vector normalVector = normalValue2;
        Vector interVector = interValue2;
      
        if(intersectionValue > 1 && intersectionValue2 >1 )
        {
          intersectionVal = intersectionValue < intersectionValue2 ? 1 : 2;
          if(intersectionVal < 2)
          {
             normalVector = normalValue;
             interVector = interValue;
          }
        }
        else if (intersectionValue > 1){
          intersectionVal = intersectionValue;
           normalVector = normalValue;
           interVector = interValue;
        }
      
				// computer color at each vertex
				//Vector N = normalValue;
				//Vector L = (light01.direction).subVector(interValue);
				//NdotL = N.dotProduct(L);
				//if (NdotL > 0.0)
				{
					//if(intersectionValue < lightDistance) // why is it not reaching the else part??
					//{
					//	pixels[ARRAY(x,y)] = Pixel(sphere01.mat.matColor);
					//}
					//else
					//{
					
					Vector N = normalVector;
					Vector L = (light01.direction).subVector(interVector);
					Vector R;
					Vector E = Vector(0,0,-1);
					Color Ka = Color(0.1,0.1,0.1);
					Color Kd = Color(0.7,0.7,0.7);
					Color Ks = Color(0.3,0.3,0.3);

					float NdotL,NdotE,RdotE;
					L = L.normalize();
					NdotL = N.dotProduct(L);
					int numlights = 1;
					int flag = 1;
					Vector localNormal;
					Color specColor,diffColor,ambColor;
					for(int i=0;i<numlights;i++)
					{
						NdotL = N.dotProduct(L);
						NdotE = N.dotProduct(E);
						// check if N.L and N.E are same signs, if negative reverse normal
						// if both are different discard the values. Flag is set for both cases
						if(NdotL >= 0 && NdotE >= 0)
						{
							localNormal = N;
							flag = 0;
						}
						else if(NdotL < 0 && NdotE < 0)
						{
							localNormal = N.negate();
							NdotL = -NdotL;
							flag = 0;
						}
						else
						{
							flag = 1;
						}
						// R = 2(N.L)N-L
						R.x = (2*NdotL*localNormal.x) - light01.direction.x; 
						R.y = (2*NdotL*localNormal.y) - light01.direction.y; 
						R.z = (2*NdotL*localNormal.z) - light01.direction.z; 
						R = R.normalize();
						RdotE = R.dotProduct(E); 
						if(RdotE < 0) RdotE = 0;if(RdotE > 1) RdotE = 1;
						if(flag == 0)
						{
							// Diffuse Summation
							diffColor.red   += light01.lightColor.red   * NdotL;
							diffColor.green += light01.lightColor.green * NdotL;
							diffColor.blue  += light01.lightColor.blue  * NdotL;
							// Specular Summation
							specColor.red   += light01.lightColor.red   * pow(RdotE,32);
							specColor.green += light01.lightColor.green * pow(RdotE,32);
							specColor.blue  += light01.lightColor.blue  * pow(RdotE,32);
						}
						else
						{
							diffColor.red += 0;diffColor.green +=0;diffColor.blue += 0;
							specColor.red += 0;specColor.green += 0;specColor.blue += 0;
						}
					}
					diffColor.red   = Kd.red   * diffColor.red ;
					diffColor.green = Kd.green * diffColor.green;
					diffColor.blue  = Kd.blue  * diffColor.blue;

					specColor.red   = Ks.red * specColor.red ;
					specColor.green = Ks.green * specColor.green;
					specColor.blue  = Ks.blue * specColor.blue;

					Color color01 = Color(sphere01.mat.matColor);
					color01.red   = color01.red    *( diffColor.red   + specColor.red);// R
					color01.green = color01.green  *( diffColor.green + specColor.green);// G
					color01.blue  = color01.blue   *( diffColor.blue  + specColor.blue);// B

					pixels[ARRAY(x,y)] = Pixel(color01);
					//}
				}
				/*else
				{
					pixels[ARRAY(x,y)] = Pixel(sphere01.mat.matColor);
				}*/
			}
			else
			{
				pixels[ARRAY(x,y)] = Pixel(Color(0.4,0.2,0.3,1.0));
			}
		}
	} 
	result = saveImage(pixels);
  
  std::cout<<"Finished rendering"<<std::endl;
  
	return SUCCESS;
}


