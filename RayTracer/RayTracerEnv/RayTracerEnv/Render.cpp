//
//  Render.cpp
//  RayTracerEnv
//
//  Created by hima on 11/14/14.
//  Copyright (c) 2014 hima. All rights reserved.
//

#include "Render.h"
#include "math.h"
#include "stdlib.h"
#include "Shader.h"
#include <cstdlib>
#include <climits>

//@todo : VC++ people, uncomment this for unblocking you.
#ifndef OS_X_ENV
#define INFINITY std::numeric_limits<double>::max()
#define M_PI 3.1427
#endif

/*  MACROS */
#define _USE_MATH_DEFINES
#define RAD_CONV_CONST (M_PI/180.0)
#define TRACER_THRESH   0.0000001 // checking for an epsilon of 10^-6(if causing problems change to 1)

//@todo move these macros to the debug headers

// Basic just for prints
#define DEBUG_MODE_NONE  0x00000000
#define DEBUG_MODE_BASIC 0x00000001
#define DEBUG_MODE_ADV   0x00000002  //no use for advanced debug mode yet..

#define CHECK_MODE_ 0
//#define ARRAY(x,y,width) (x+(y * width))
/*  LOCAL STRUCTS, both non-stati and static structs */
typedef struct{

  int debug_mode;

}render_debug_s;

void antiAliasing(infinity_render_s* thisObj);

int ARRAY(int x,int y,int width){
	return (x+(y*width));
}

#if 0
/* LOCAL OBJECTS, to be referenced only in this file */
render_debug_s debug_mode = {
  .debug_mode = 0x0
};
#endif
double getAspectRatio(infinity_render_s* thisObj)
{
  return thisObj->aspectRatio;
}

void setAspectRatio(infinity_render_s* thisObj)
{
  if(thisObj->width > 0.000000000001){
    thisObj->aspectRatio = (1.0 * thisObj->height)/(1.0 * thisObj->width);
  }
  return;
}

void setPixelParams(infinity_render_s* thisObj, double camFOVDeg)
{
  double rad = 0.0;
  //First get the camera FOV in rad
  setAspectRatio(thisObj);
  
  rad = tan((camFOVDeg/2.0) * RAD_CONV_CONST);
  thisObj->pixelWidth  =  (rad * 2.0)/(thisObj->width -1.0);
  thisObj->pixelHeight =  ((rad * thisObj->aspectRatio) * 2.0)/(thisObj->height -1.0);
  
  return;
}

void setRenderSize(infinity_render_s* thisObj, int height, int width){

  thisObj->height = height;
  thisObj->width = width;

  /*  update the aspect ratio */
  setAspectRatio(thisObj);

  /*  Now reallocate memory used by the renderer object for pixel storage */
  thisObj->pixels = (Pixel*)malloc(sizeof(Pixel)*width*height);

  if(thisObj->pixels == NULL)
  {
    std::cout<<"Unable to allocate memory for renderer, bailing out"<<std::endl;
    exit(-1);
  }
  
  return;
}

/* The obj to render is the one with the lowest distance */
#if 0
GeomObj* findObjToRender
(
  const int        numObjects,
  const GeomObj*   objsToRender
)
{
  GeomObj* objToRender = NULL;
  double minDistance   = INFINITY;

  for(int o = 0; o< numObjects; o++)
  {
    if(minDistance < objsToRender[o]->intersectionValue)
    {
      objToRender = objsToRender[o];
    }
  }
  return objToRender;
}
#endif

void antiAliasing( infinity_render_s* thisObj){
	
  /* @todo - this is problematic, can someone explain what this is doing here? */
	//Pixel *pixels = new Pixel[thisObj->width * thisObj->height];
	for(int i=0;i<thisObj->width;i++)
		for(int j=0;j<thisObj->height; j++){
			int count = 0;
			Color c = Color(0,0,0,0);
			if(i-1 >=0 && j-1 >= 0){
				c.addColor(thisObj->pixels[ARRAY(i-1,j-1,thisObj->width)].pixelColor);
				count ++;
			}
			if(i-1 >=0){
				c.addColor(thisObj->pixels[ARRAY(i-1,j,thisObj->width)].pixelColor);
				count ++;
			}
			if(i-1 >=0 && j+1 < thisObj->height){
				c.addColor(thisObj->pixels[ARRAY(i-1,j+1,thisObj->width)].pixelColor);
				count ++;
			}
			if(j-1 >= 0){
				c.addColor(thisObj->pixels[ARRAY(i,j-1,thisObj->width)].pixelColor);
				count ++;
			}
			if(j+1 < thisObj->height){
				c.addColor(thisObj->pixels[ARRAY(i,j+1,thisObj->width)].pixelColor);
				count ++;
			}
			if(i+1 < thisObj->width && j+1 < thisObj->height){
				c.addColor(thisObj->pixels[ARRAY(i+1,j+1,thisObj->width)].pixelColor);
				count ++;
			}
			if(i+1 < thisObj->width){
				c.addColor(thisObj->pixels[ARRAY(i+1,j,thisObj->width)].pixelColor);
				count ++;
			}

			if(i+1 < thisObj->width && j-1 >= 0){
				c.addColor(thisObj->pixels[ARRAY(i+1,j-1,thisObj->width)].pixelColor);
				count ++;
			}
			count++;
			c.addColor(thisObj->pixels[ARRAY(i,j,thisObj->width)].pixelColor);
			c.avgColor(count);
			thisObj->pixels[ARRAY(i,j,thisObj->width)].setColor(c);
		}
	return;
}

Color PositionColors[] = {
  Color(0.68,0.61,0.80,1.0),
  Color(0.72,0.63,0.82,1.0),
  Color(0.76,0.65,0.84,1.0),
  Color(0.80,0.67,0.86,1.0),
  Color(0.84,0.69,0.88,1.0),
  Color(0.88,0.71,0.90,1.0)
};

Color* getObjColorOnPosition(int j)
{
  Color *ret_color = NULL;
  ret_color = ( j<= 40 ? &PositionColors[0] : ( j <= 80 ? &PositionColors[1] :  \
    ( j <=120 ? &PositionColors[2] : (j <= 160 ? &PositionColors[3] : j <= 200 ?  \
      &PositionColors[4] : &PositionColors[5]))) );

  return ret_color;

}/* getObjColorOnPosition */

/*  For now we only pass the objects, perhaps can add a pointer to a camera object */
void renderObjects
(
  infinity_render_s * thisObj, 
  GeomObj     * objsToRender[],   //@todo could also be : renderObjects
  Camera      * camera,
  int           numObjects,
  Light       * lights,        //@todo, make this a typedef color light
  int           numLights
)
{
  /* Local variables */
  double horizScaleCeoff = 0.0, vertScaleCoeff  = 0.0;
  Vector horizOffset, vertOffset;
  Ray    primaryRay, secondaryRay, tempRay;
  GeomObj* objRend = NULL;
  //double   intersectValue = INFINITY;
  Color  currentColor, reflectionColor;
  int counter = 0;

  /* Initializations */
  primaryRay.origin = camera->position;
  primaryRay.currentRefractiveIndex = REFRACTIVE_INDEX_AIR;

  for(int obj = 0;obj<numObjects;obj++)
  {
	  objsToRender[obj]->objectId = counter++;
  }

  /* Main processing */
  /* Render line by line, for all lines */
  for(int i =0; i < thisObj->width; i++)
  {
    //std::cout<<"i="<<i<<std::endl;   <--- annoying

    horizScaleCeoff = (i*thisObj->pixelWidth) - (camera->width/2.0);
    horizOffset     = camera->camX.scalarMult(horizScaleCeoff);
    /*  Now render this line  */
    for(int j = 0; j < thisObj->height;j++)
    {
      printf("%cRendering: %dx%d",13,i,j);
      fflush(stdout);
      objRend         = NULL;
      vertScaleCoeff  = (j*thisObj->pixelHeight) - (camera->height/2.0);
      vertOffset = camera->camY.scalarMult(vertScaleCoeff);
      primaryRay.direction = (camera->camZ.addVector(horizOffset)).addVector(vertOffset);
      primaryRay.direction = primaryRay.direction.normalize();

	    bool objFlag = false;

      objFlag = trackRay(primaryRay, &currentColor, lights, numLights, objsToRender, numObjects, 0);

	    if(objFlag == true){
		    thisObj->pixels[ARRAY(i,j,thisObj->width)].setColor(currentColor);
	    }
      else{  //Set the default color to something?
        thisObj->defaultColor = *(getObjColorOnPosition(j));
        thisObj->pixels[ARRAY(i,j,thisObj->width)].setColor(thisObj->defaultColor);
      }
    }/* End of display width, ie. each raster line */
  }/*  End of display height */
  
  printf("\n");
  /* To smooth-en out the edges */
  //antiAliasing(thisObj);
  return;
}/* renderObjects */

bool trackRay(Ray incomingRay, Color *thisColor, Light *lights, int numLights, GeomObj *objsToRender[], int numObjects, int currentDepth)
{
  if (currentDepth > INTERACTION_DEPTH)
    return false;

  Ray reflectedRay, refractedRay;
  GeomObj *objRend;
  bool objFlag = false;
  int currentObjId;
  Color reflectionColor, refractionColor;
	Vector objIntersectionVector, objNormalVector;
	double objIntersectionValue;
  double dotProd, theta_i, theta_t;
  float n1, n2;
  bool doRefraction;
  double R_total, T_total;
  objRend = intersection(objsToRender, numObjects, incomingRay);
	//tempRay = incomingRay;
	if(objRend != NULL)
	{
    currentObjId  = objRend->objectId;
    objIntersectionVector = objRend->objIntersection;
    objIntersectionValue = objRend->intersectionValue;
    objNormalVector = objRend->objNormal;
  	objFlag = true;
    incomingRay.direction = incomingRay.direction.normalize();
    objNormalVector = objNormalVector.normalize();
    n1 = incomingRay.currentRefractiveIndex;
    n2 = objRend->material.refractionIndex;

    if (n1 == 1.5 && n2 == 1.5) {
      n2 = 1;
      objNormalVector = objNormalVector.negate();
    }

	getColor(lights, numLights, objRend, thisColor, objsToRender, &incomingRay, numObjects);

    reflectedRay.origin = objIntersectionVector; reflectedRay.currentRefractiveIndex = incomingRay.currentRefractiveIndex;
    //Using reflection equations from http://www.cs.jhu.edu/~cohen/RendTech99/Lectures/Ray_Tracing.bw.pdf
    dotProd = (objNormalVector.dotProduct(incomingRay.direction));
    reflectedRay.direction = (incomingRay.direction.subVector(objNormalVector.scalarMult(dotProd*2)));
	reflectedRay.direction = reflectedRay.direction.normalize();

  if (n1 == 1.5 || n2 == 1.5) {
      refractedRay.origin = objIntersectionVector; refractedRay.currentRefractiveIndex = n2;
      //Using refraction equations from http://graphics.stanford.edu/courses/cs148-10-summer/docs/2006--degreve--reflection_refraction.pdf
      theta_i = acos(objNormalVector.dotProduct(incomingRay.direction.negate()));
      theta_t = asin((n1/n2)*sin(theta_i));
      double innerTerm1 = 1 - pow(sin(theta_t) , 2);
      if (innerTerm1 > 0) {
        double innerTerm2 = ((n1/n2) * cos(theta_i) - sqrt(innerTerm1));
        refractedRay.direction = incomingRay.direction.scalarMult(n1/n2).addVector(objNormalVector.scalarMult(innerTerm2));
        refractedRay.direction = refractedRay.direction.normalize();
        doRefraction = true;
          double R0 = pow(((n1-n2)/(n1+n2)), 2);
          if (n1 <= n2)   {
              R_total = R0 + (1 - R0)*(pow((1 - cos(theta_i)), 5));
          }
          else    {
              R_total = R0 + (1 - R0)*(pow((1 - cos(theta_t)), 5));
          }
      }
      else    {
        doRefraction = false;
        R_total = 1;
      }
      T_total = 1 - R_total;
  }
  else  {
    doRefraction = false;
    R_total = objRend->material.reflectionParameter;
  }

    bool reflectedRayReturns = trackRay(reflectedRay, &reflectionColor, lights, numLights, objsToRender, numObjects, (currentDepth+1));

    if (reflectedRayReturns == true)  {
      thisColor->red   += reflectionColor.red * R_total;
	  thisColor->green += reflectionColor.green * R_total;
	  thisColor->blue  += reflectionColor.blue * R_total;
    }
    
    if (doRefraction == true) {
      bool refractedRayReturns = trackRay(refractedRay, &refractionColor, lights, numLights, objsToRender, numObjects, (currentDepth+1));;

      if (refractedRayReturns == true)  {
        thisColor->red   += refractionColor.red * T_total;
        thisColor->green += refractionColor.green * T_total;
        thisColor->blue  += refractionColor.blue * T_total;
      }
    }

		/* now that we have the object to render, find the color for this pixel */
		/*for(int r = 0; r < reflectionDepth; r++)
		{
			//cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)
			//secondaryRay.origin    = objIntersectionVector.subVector(tempRay.origin);
      secondaryRay.origin    = objRend->objIntersection.subVector(tempRay.origin);
			secondaryRay.direction = (objRend->objNormal.scalarMult(2*(objRend->objNormal.dotProduct(tempRay.origin))).subVector(tempRay.origin));
			secondaryRay.direction = secondaryRay.direction.normalize();

      reflectedRay.origin = tempRay.direction.scalarMult(objRend->intersectionValue);

			// 2(N.L)*N - L where N is normal and L is negative ray direction
			double dotProd = (objNormalVector.dotProduct(tempRay.direction.negate()));
			reflectedRay.direction = (objNormalVector.scalarMult(dotProd*2)).subVector(tempRay.direction.negate());
			reflectedRay.direction = reflectedRay.direction.normalize();

			objRend = intersection(objsToRender, numObjects, reflectedRay);
			tempRay = reflectedRay;

			if(objRend != NULL && objRend->objectId!=currentObjId )
			{
				getColor(lights, numLights, objRend, &reflectionColor, objsToRender, &tempRay, numObjects);
				thisColor->red   += reflectionColor.red*0.2;//0.2 is the reflection prcentage
				thisColor->green += reflectionColor.green*0.2;
				thisColor->blue  += reflectionColor.blue*0.2;
			}
			else
			{
				break;
			}
			currentObjId  = objRend->objectId;
		}
    */
  }
  return objFlag;
}

GeomObj* intersection(GeomObj* objsToRender[],int numObjects,Ray currentRay)
{
	GeomObj* objRend = NULL, *currentObj;
	/*  Now loop through all the objects */
      double intersectValue = INFINITY;
      for(int obj = 0;obj<numObjects;obj++)
      {
        currentObj = objsToRender[obj];
        //Perform the ray intersection with this object
        currentObj->findIntersection(currentRay);
        if(currentObj->intersectionValue < intersectValue &&
           currentObj->intersectionValue > TRACER_THRESH )
        {
          objRend = objsToRender[obj];
          intersectValue = objRend->intersectionValue;
        }
      } /* End of obj loop */
	  return objRend;
}
