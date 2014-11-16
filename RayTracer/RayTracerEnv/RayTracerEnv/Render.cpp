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
//#define INFINITY std::numeric_limits<double>::max()
//#define M_PI 3.1427

/*  MACROS */
#define _USE_MATH_DEFINES
#define RAD_CONV_CONST (M_PI/180.0)
#define TRACER_THRESH   1.0

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
  double horizScaleCeoff = 0.0;
  double vertScaleCoeff  = 0.0;
  Vector horizOffset;//     = {0,0,0};
  Vector vertOffset;//      = {0,0,0};
  Ray    primaryRay;//      = {};
  Ray    secondaryRay;//    = {};
  GeomObj* objRend = NULL;
  double   intersectValue = std::numeric_limits<double>::max();
  Color  thisColor;

  /* Initializations */
  primaryRay.origin = camera->position;

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

      /*  Now loop through all the objects */
      intersectValue = INFINITY;
      for(int obj = 0;obj<numObjects;obj++)
      {
        //Perform the ray intersection with this object
        objsToRender[obj]->findIntersection(primaryRay);
        if(objsToRender[obj]->intersectionValue < intersectValue &&
           objsToRender[obj]->intersectionValue > TRACER_THRESH )
        {
          objRend = objsToRender[obj];
          intersectValue = objRend->intersectionValue;
        }
      } /* End of obj loop */

      /* now that we have the object to render, find the color for this pixel */
      if(objRend !=NULL)
      {
      
        //std::cout<<i<<","<<j<<" ";
        if(i == 90 && j == 260)
        {
          std::cout<<"BP"<<std::endl;
        }
      
        getColor(lights,numLights,objRend,&thisColor);
        thisObj->pixels[ARRAY(i,j,thisObj->width)].setColor(thisColor);
      }
      else{
        //Set the default color to something?
        thisObj->pixels[ARRAY(i,j,thisObj->width)].setColor(thisObj->defaultColor);
      }

    }/* End of display width, ie. each raster line */
  }/*  End of display height */
  return;
}/* renderObjects */


