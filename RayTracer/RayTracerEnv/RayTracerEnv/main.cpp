//
//  main.cpp
//  RayTracerEnv
//
//  Created by hima on 11/11/14.
//  Copyright (c) 2014 hima. All rights reserved.
//  @original code from uthara (thelagar@usc.edu)

/* Raytracer Framework for CS580 Project
*/



#define MAX_LIGHTS 10
#define MAX_SPHERES 15
#define MAX_PLANES 1
#define MAX_TRIANGLES 5
#define MAX_COMPLEX_OBJECTS 5
#define MAX_OBJ_TO_RENDER 20

#include "Shader.h"
#include "TracerAppBSP.h"


static infinity_bsp_s infinity_bsp;
static infinity_tracer_app_data_s tracer_app;

#if 0
/* Stores all the elemental structures needed for initializing the app data */
static infinity_bsp_s infinity_bsp =
{
  .dispWidth = XRES,
  .dispHeight = YRES,
  
  .defaultCamFOV = 45,
  
  .camPosition = Vector(0,-2.5,10),
  .camLook      = Vector(0,0,3.0),

  
  .numSpheres = 5,
  .spheres = {
    Sphere(Vector(0.0,1.2,0.0),0.5,Material(Color(0.2,0.00,0.0,1.0))),
    Sphere(Vector(3.0,1.2,0.0),0.5,Material(Color(0.0,0.2,0.0,1.0))),
    Sphere(Vector(-3.0,1.2,0.0),0.5,Material(Color(0.0,0.0,0.25,1.0))),
    Sphere(Vector(-1.5,2.4,0.0),0.5,Material(Color(0.3,0.0,0.25,1.0))),
    Sphere(Vector(1.5,0.0,0.0),1.0,Material(Color(0.35,0.03,0.1,1.0)))
    },

  .numPlanes = 0,
  .planes = {Plane(Vector(0.0,1.0,0.0),-1.0,Material(Color(0.5,0.0,0.2)))}
  
};

static infinity_tracer_app_data_s tracer_app = {

  .appCamera = Camera(infinity_bsp.camPosition,infinity_bsp.camLook),
  
  .numObjsToRender = 0,
  .objectsToRender = {NULL},

  .infinityRender =
    {
      .height = infinity_bsp.dispHeight,
      .width  = infinity_bsp.dispWidth,
      .aspectRatio = 0.0,
      .pixelWidth = 0.0,
      .pixelHeight = 0.0,
      .cameraWidth = 0.0,
      .cameraHeight = 0.0,
      .pixels = NULL,
      .defaultColor = Color(1.0,1.0,1.0,1.0),
    },
  
    
  .numLights = 1,
  .defLights =
    {
      Light( Vector(0.0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0)),
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0))
    },
};

#endif


static char fileName1[] = "obj_files/pawn.obj";
static char fileName2[] = "obj_files/pillar01.obj";
static char fileName3[] = "obj_files/pillar02.obj";

void pre_initialize_everything()
{

  init_shader();

  infinity_bsp.dispWidth = XRES;
  infinity_bsp.dispHeight = YRES;
  
  infinity_bsp.defaultCamFOV = 50;
  
  //infinity_bsp.camPosition = Vector(0,-2.5,10);
  //infinity_bsp.camLook      = Vector(0,0,3.0);

  // CAMERA FOR TESTING COMPLEX OBJECT

  //infinity_bsp.camPosition = Vector(5,5,5);
  //infinity_bsp.camLook      = Vector(1.0,1.0,1.0);

  infinity_bsp.camPosition = Vector(-0.3,0.0,23);
  infinity_bsp.camLook      = Vector(0.1,-2.0,1.0);

  
  // CAMERA FOR ANTIALISING / SHADING
  //infinity_bsp.camPosition = Vector(0,0,5);
  //infinity_bsp.camLook      = Vector(0,0,1.0);

  infinity_bsp.numSpheres = 8;
  infinity_bsp.spheres[0] = Sphere(Vector(-4.0,1.0,-9.0),1.4,Material(Color(0.45,0.28,0.4,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[1] = Sphere(Vector(-1.4,1.0,-13.0),1.4,Material(Color(0.45,0.2,0.05,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[2] = Sphere(Vector(1.5, 1.0,-13.0),1.4,Material(Color(0.45,0.28,0.4,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[3] = Sphere(Vector(4.1, 1.0,-9.0),1.4,Material(Color(0.45,0.2,0.05,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[4] = Sphere(Vector(-4.0,1.0,1.0),1.4,Material(Color(0.45,0.2,0.05,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[5] = Sphere(Vector(-1.3,1.0,3.0),1.4,Material(Color(0.45,0.28,0.4,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[6] = Sphere(Vector(1.4, 1.0,3.0),1.4,Material(Color(0.45,0.2,0.05,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  infinity_bsp.spheres[7] = Sphere(Vector(4.1, 1.0,1.0),1.4,Material(Color(0.45,0.28,0.4,1.0), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));


  infinity_bsp.numPlanes = 1;
  infinity_bsp.planes[0] = Plane(Vector(0.0,1.0,0.0),2.4,Material(Color(0.5,0.0,0.2), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));// having z value in plane normal is like rotating it in z
  
  infinity_bsp.numTriangles = 0;
  infinity_bsp.triangles[0] = Triangle(Vector(0.0,0.0,0.0),Vector(0.0,1.0,0.0),Vector(1.0,0.0,0.0));

  infinity_bsp.numComplexObjects = 3;
  infinity_bsp.complex_objects[0] = ComplexObject(fileName1, Material(Color(0.9,0.4,0.2), Color(0.1,0.1,0.1), Color(0.6,0.6,0.6), Color(0.25,0.25,0.3), 0.2, 1.5));
  infinity_bsp.complex_objects[1] = ComplexObject(fileName2 , Material(Color(0.1,0.15,0.35), Color(0.1,0.1,0.1), Color(0.3,0.3,0.3), Color(0.2,0.2,0.2), 0.2, 1.5));
  infinity_bsp.complex_objects[2] = ComplexObject(fileName3, Material(Color(0.1,0.15,0.35), Color(0.1,0.1,0.1), Color(0.4,0.4,0.4), Color(0.2,0.2,0.2), 0.2, 1.5));
  //infinity_bsp.complex_objects[0] = ComplexObject("obj_files/cube.obj" , Material(Color(0.9,0.9,0.9), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2, 1.5));
  //infinity_bsp.complex_objects[4] = ComplexObject("obj_files/floor.obj", Material(Color(0.2,0.2,0.2), Color(0.1,0.1,0.1), Color(0.7,0.7,0.7), Color(0.3,0.3,0.3), 0.2,1.5) );

  tracer_app.appCamera = Camera(infinity_bsp.camPosition,infinity_bsp.camLook);
  
  tracer_app.numObjsToRender = 0;
  //tracer_app.objectsToRender[0] = {};

  //.infinityRender =
  
   tracer_app.infinityRender.height = infinity_bsp.dispHeight;
   tracer_app.infinityRender.width  = infinity_bsp.dispWidth;
   tracer_app.infinityRender.aspectRatio = 1.0 * (double)infinity_bsp.dispHeight/ (double)infinity_bsp.dispWidth;
   tracer_app.infinityRender.pixelWidth = 0.0;
   tracer_app.infinityRender.pixelHeight = 0.0;
   tracer_app.infinityRender.cameraWidth = 0.0;
   tracer_app.infinityRender.cameraHeight = 0.0;
   tracer_app.infinityRender.pixels = NULL;
   tracer_app.infinityRender.defaultColor = Color(0.8,0.9,0.95,1.0);
    
  tracer_app.numLights = 2;
  
  tracer_app.defLights[0] =
  // LIGHTS - ANTIALIASING, SHADING
  Light( Vector(2.0,0.0,2.0),Color(0.8,0.8,0.8));
  // LIGHTS - COMPLEX OBJECTS 
  //Light( Vector(1.0,2.0,0.5),Color(0.8,0.8,0.8)); 
  // Light(Vector(4,-10.0,3.0),Color(1.0,1.0,1.0));
  
  tracer_app.defLights[1] =
      Light( Vector(-2.0,-2.0,-2.0),Color(0.4,0.4,1.0));
  tracer_app.defLights[2] =
      Light( Vector(0.0 , 0.0, 2.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[3] =
      Light( Vector(0.0 , 0.0,-2.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[4] =
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[5] =
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[6] =
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[7] =
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[8] =
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0));
  tracer_app.defLights[9] =
      Light( Vector(0,0.0,-10.0),Color(1.0,1.0,1.0));

  
  return;
}

//Next step is checkerboard...
// create 10^-6 check for intersection value greater than 0.000001
// for reflection check on:
// http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-1-writing-a-simple-raytracer/

#include "math.h"

bool initialize_tracer_app()
{
  bool ret_val = true;

  /* do any initializations the VC++ loader cannot do */
  pre_initialize_everything();
  
  tracer_app.appCamera.setFOV(infinity_bsp.defaultCamFOV,((double)infinity_bsp.dispHeight/(double)infinity_bsp.dispWidth));
  
  int o = 0;
  /* First add all the spheres */
  for(int s= 0; (s< infinity_bsp.numSpheres) && (o < MAX_OBJ_TO_RENDER); s++, o++)
  {
    tracer_app.objectsToRender[o] = (GeomObj*)&infinity_bsp.spheres[s];
  }
  
  /* Now add all the planes */
  for(int p= 0; (p< infinity_bsp.numPlanes) && (o < MAX_OBJ_TO_RENDER); p++, o++)
  {
    tracer_app.objectsToRender[o] = (GeomObj*)&infinity_bsp.planes[p];
  }
  
  /* Now add all the triangles */
  for(int t= 0; (t< infinity_bsp.numTriangles) && (o < MAX_OBJ_TO_RENDER); t++, o++)
  {
    tracer_app.objectsToRender[o] = (GeomObj*)&infinity_bsp.triangles[t];
  }

  /* Now add all the complex objects */
  for(int c= 0; (c< infinity_bsp.numComplexObjects) && (o < MAX_OBJ_TO_RENDER); c++, o++)
  {
	tracer_app.objectsToRender[o] = (GeomObj*)&infinity_bsp.complex_objects[c];
  }

  tracer_app.numObjsToRender = o;
  /*  Now initialize the renderer to use the camera's FOV */
  tracer_app.infinityRender.pixels = NULL;
  setRenderSize(&tracer_app.infinityRender,infinity_bsp.dispHeight,infinity_bsp.dispWidth);
  setPixelParams(&tracer_app.infinityRender,tracer_app.appCamera.FOV);
  
  return ret_val;
} /* initialize_tracer_app */


int main
(
  int     argc,
  char ** argv
)
{
  //GeomObj * objsToRender = NULL;
  
  (void)initialize_tracer_app();
	
  std::cout<<"Begin rendering ..."<<std::endl;

  //Parser would read obj here
  //objsToRender = getParserObjs();
  
  renderObjects(
    &tracer_app.infinityRender,
     tracer_app.objectsToRender,
    &tracer_app.appCamera,
     tracer_app.numObjsToRender,
     tracer_app.defLights,
     tracer_app.numLights);

  
  saveImage(tracer_app.infinityRender.pixels);
  
  std::cout<<"Finished rendering"<<std::endl;
  
	return SUCCESS;
}


