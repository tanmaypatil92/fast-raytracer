#ifndef _COMPLEXOBJECT_H_
#define _COMPLEXOBJECT_H_

#include "Ray.h"
#include "Vector.h"
#include "Material.h"
#include "OBJParser.h"
#include "triangle.h"
#include "GeometricObject.h"
#include "BoundingBox.h"
#include <stddef.h>
#include <float.h>

#define MAX_TRI 6500
class Oct
{
public:
	BoundingBox bb;
	
	int n_tri_inside;
	int* tri_index_list;//[MAX_TRI];

	Oct* sub_oct[8];

	Oct()
	{
		n_tri_inside = 0;
		//for(int i = 0; i<MAX_TRI ;i++ )
			//tri_index_list[i] = -1;

		for(int i = 0; i<8 ;i++ )
			sub_oct[i] = NULL;
	}
};
class return_data
{
public:	
	int tri_index;
	float min_intersectionVal;

	return_data()
	{
		tri_index = -1; 
		min_intersectionVal = FLT_MAX;
	}
};
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
	Vector VertexNormal_to_Vector(VertexNormal vertex);

	void OutputFaces();

	double complexIntersection(Ray);
	double complexIntersection_faster(Ray);

	double findIntersection(Ray ray);

	//  OCT TREE 
	int n_tri;
	Triangle* tri_list[MAX_TRI];
	Oct* head_oct;
	
	void get_triangles();
	
	int curr_oct_depth, max_oct_depth;
	Oct* initialize_oct(BoundingBox box);
	return_data complexIntersection_recursive(Oct* oct ,Ray ray);
};

#endif