#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"

class Material{
	public:
	Color matColor;
  Color Ka, Kd, Ks;
  float reflectionParameter, refractionIndex;
	Material();
	Material(Color, Color, Color, Color, float, float);
};
#endif