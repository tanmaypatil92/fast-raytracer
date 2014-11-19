//
//  Render.h
//  RayTracerEnv
//
//  Created by hima on 11/14/14.
//  Copyright (c) 2014 hima. All rights reserved.
//

#ifndef __RayTracerEnv__Render__
#define __RayTracerEnv__Render__

#define REFLECTION_DEPTH 1

#include <stdio.h>
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

typedef struct{

  /* Ideally these belong in teh display class object */
  int height;
  int width;
  double aspectRatio;

  /* Camera based properties  */
  double pixelWidth;
  double pixelHeight;
  /* @todo Hack for now, store the camera's properties here */
  double cameraWidth;
  double cameraHeight;

  Pixel *pixels;

  Color defaultColor;
  
}infinity_render_s;

void setRenderSize(infinity_render_s* thisObj, int height, int width);
void setPixelParams(infinity_render_s* thisObj, double camFOVDeg);

void renderObjects
(
  infinity_render_s * thisObj, 
  GeomObj     * objsToRender[],  //@todo should be : renderObjects[] 
  Camera      * camera,
  int           numObjects,
  Light       * lights,        //@todo, make this a typedef color light
  int           numLights
);

bool trackRay(Ray incidentRay, Color *thisColor, Light *lights, int numLights, GeomObj *objsToRender[], int numObjects, int currentDepth);
GeomObj* intersection(GeomObj* objsToRender[],int numObjects,Ray currentRay);

#endif /* defined(__RayTracerEnv__Render__) */
