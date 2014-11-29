#ifndef _GEOM_OBJ_H
#define _GEOM_OBJ_H

#include "Ray.h"
#include "Vector.h"
#include "Material.h"
#include "OBJParser.h"

class GeomObj
{
	public:
	double intersectionValue;
	Material material;
	Vector objNormal;
	Vector objIntersection;
	bool texture_defined;
	VertexTexture tex;

	int objectId;
	//virtual dumdum() = 0;

	

	virtual double findIntersection(Ray ray, int intersectionSide)=0;
	
	GeomObj()
	{
		texture_defined = false;
	}
};

#endif

