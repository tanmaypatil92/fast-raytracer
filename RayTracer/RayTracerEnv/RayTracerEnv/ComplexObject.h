#ifndef _COMPLEXOBJECT_H_
#define _COMPLEXOBJECT_H_

#include "Ray.h"
#include "Vector.h"
#include "Material.h"
#include "OBJParser.h"
#include "triangle.h"
#include "GeometricObject.h"
#include "BoundingBox.h"

class ComplexObject : public GeomObj
{
	public:
	
	char obj_filename[40];
	OBJObject myOBJ;

	BoundingBox box;

	Material mat;

	Vector complexInter;
	Vector complexNormal;

	ComplexObject();
	ComplexObject(char* obj_filename,Material m);
	
	Vector Vertex_to_Vector(Vertex vertex);
	void OutputFaces();

	double complexIntersection(Ray);
	double findIntersection(Ray ray);
};

#endif