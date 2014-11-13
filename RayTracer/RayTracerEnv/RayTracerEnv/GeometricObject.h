#ifndef _GEOM_OBJ_H
#define _GEOM_OBJ_H

#include "Ray.h"
#include "Vector.h"
#include "Material.h"

class GeomObj
{
	public:
	double intersectionValue;
	//virtual dumdum() = 0;

	virtual double findIntersection(Ray ray)=0;
	
};

#endif

