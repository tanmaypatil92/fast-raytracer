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
#define INFINITY std::numeric_limits<double>::max()
#define M_PI 3.1427

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
  double   intersectValue = INFINITY;
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
    horizScaleCeoff = (i*thisObj->pixelWidth) - (camera->width/2.0);
    horizOffset     = camera->camX.scalarMult(horizScaleCeoff);
    /*  Now render this line  */
    for(int j = 0; j < thisObj->height;j++)
    {
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
        thisObj->pixels[ARRAY(i,j,thisObj->width)].setColor(thisObj->defaultColor);
      }
    }/* End of display width, ie. each raster line */
  }/*  End of display height */
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
  double dotProd, theta_i; 
  float n1, n2;
  bool doRefraction;

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

		getColor(lights, numLights, objRend, thisColor, objsToRender, &incomingRay, numObjects);

    reflectedRay.origin = objIntersectionVector; reflectedRay.currentRefractiveIndex = incomingRay.currentRefractiveIndex;
    //Using reflection equations from http://www.cs.jhu.edu/~cohen/RendTech99/Lectures/Ray_Tracing.bw.pdf
    dotProd = (objNormalVector.dotProduct(incomingRay.direction));
    reflectedRay.direction = (incomingRay.direction.subVector(objNormalVector.scalarMult(dotProd*2)));
		reflectedRay.direction = reflectedRay.direction.normalize();

    refractedRay.origin = objIntersectionVector; refractedRay.currentRefractiveIndex = objRend->material.refractionIndex;
    //Using refraction equations from http://www.flipcode.com/archives/reflection_transmission.pdf
    theta_i = acos(objNormalVector.dotProduct(incomingRay.direction.negate()));
    double innerTerm1 = 1 - pow((n1/n2*sin(theta_i)) , 2);
    if (innerTerm1 > 0) {
      double innerTerm2 = ((n1/n2) * cos(theta_i) + sqrt(innerTerm1));
      refractedRay.direction = incomingRay.direction.scalarMult(n1/n2).subVector(objNormalVector.scalarMult(innerTerm2));
      doRefraction = true;
    }
    else
      doRefraction = false;

    bool reflectedRayReturns = trackRay(reflectedRay, &reflectionColor, lights, numLights, objsToRender, numObjects, (currentDepth+1));

    if (reflectedRayReturns == true)  {
      thisColor->red   += reflectionColor.red * objRend->material.reflectionParameter;
			thisColor->green += reflectionColor.green * objRend->material.reflectionParameter;
			thisColor->blue  += reflectionColor.blue * objRend->material.reflectionParameter;
    }

    /*
    if (doRefraction == true) {
      bool refractedRayReturns = trackRay(refractedRay, &refractionColor, lights, numLights, objsToRender, numObjects, (currentDepth+1));;

      if (refractedRayReturns == true)  {
        thisColor->red   += refractionColor.red * objRend->material.reflectionParameter;
			  thisColor->green += refractionColor.green * objRend->material.reflectionParameter;
			  thisColor->blue  += refractionColor.blue * objRend->material.reflectionParameter;
      }
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

