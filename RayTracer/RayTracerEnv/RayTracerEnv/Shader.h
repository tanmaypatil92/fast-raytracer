//
//  Shader.h
//  RayTracerEnv
//
//  Created by hima on 11/14/14.
//  Copyright (c) 2014 hima. All rights reserved.
//

#ifndef __RayTracerEnv__Shader__
#define __RayTracerEnv__Shader__

#include <stdio.h>
#include "Light.h"
#include "GeometricObject.h"

void getColor
(
  Light   * lights,
  int       numLights,
  GeomObj * objToRender,
  Color   * returnColor
);

void init_shader();
#endif /* defined(__RayTracerEnv__Shader__) */
