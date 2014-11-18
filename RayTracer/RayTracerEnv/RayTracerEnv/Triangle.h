#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Ray.h"
#include "Vector.h"
#include "Material.h"
#include "OBJParser.h"
#include "GeometricObject.h"

class Triangle : public GeomObj
{
	public:
	
	Vector v0,v1,v2;
	double distance;
	
	Material mat;

	Vector triInter;
	Vector triNormal;

	Triangle();
	Triangle(Vector,Vector,Vector);
	Triangle(OBJObject,int);
	Vector Vertex_to_Vector(Vertex vertex);
	
	double triIntersection(Ray);
	double findIntersection(Ray ray);
};

#endif