//
//  TracerAppBSP.h
//  RayTracerEnv
//
//  Created by hima on 11/15/14.
//  Copyright (c) 2014 hima. All rights reserved.
//

#ifndef RayTracerEnv_TracerAppBSP_h
#define RayTracerEnv_TracerAppBSP_h
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

typedef struct
{

  Camera appCamera;
  
  int      numObjsToRender;
  GeomObj* objectsToRender[MAX_OBJ_TO_RENDER];

  infinity_render_s infinityRender;
  
  /* The actual number of lights to initialize*/
  int numLights;
  Light defLights[MAX_LIGHTS];

}infinity_tracer_app_data_s;

typedef struct
{

  int dispWidth;
  int dispHeight;
  
  float defaultCamFOV;
  
  Vector camPosition;
  Vector camLook;
  //Camera defCam;
  

  
  
  int numSpheres;
  Sphere spheres[MAX_SPHERES];
  
  int numPlanes;
  Plane planes[MAX_PLANES];
  
} infinity_bsp_s;

#endif
