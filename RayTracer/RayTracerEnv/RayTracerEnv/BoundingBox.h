#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include "Ray.h"
#include "Vector.h"
#include "Sphere.h"
#include "OBJParser.h"
#include "GeometricObject.h"

class BoundingBox
{
	public:
	
	double left_bound, right_bound, top_bound, bottom_bound, front_bound, back_bound;
	
	bool Intersection(Ray r);

	BoundingBox();
	void SetBounds_Sphere(Vector center, double radius);
	void SetBounds_ComplexObject(OBJObject myOBJ);
};

#endif