#ifndef _PALNE_H_
#define _PALNE_H_

#include "Ray.h"
#include "Vector.h"
#include "Material.h"
#include "GeometricObject.h"

class Plane : public GeomObj
{
	public:
	double distance;
	Vector normal;
	Material mat;
	Plane();
	Plane(Vector,double);
	Plane(Vector,double,Material);
  double findIntersection(Ray r);
};

#endif