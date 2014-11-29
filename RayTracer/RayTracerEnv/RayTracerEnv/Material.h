#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Color.h"

class Material{
	public:
	Color matColor;
  Color Ka, Kd, Ks;
  char fileName[40];
  char matType[40];
  int reset;
  float reflectionParameter, refractionIndex;
  int specularPower;
	Material();
	Material(Color, Color, Color, Color, int, float, float);
	Material(Color, Color, Color, Color, int, float, float, char *);
	Material(Color, Color, Color, Color, int, float, float, char *, char *);
};
#endif