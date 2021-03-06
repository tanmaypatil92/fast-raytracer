#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Ray.h"
#include "Vector.h"
#include "Material.h"
#include "GeometricObject.h"

#define ProceduralTexture 1

class Sphere : public GeomObj
{
	public:
	Vector center;
	double radius;
	Material mat;
	Vector sphereNormal;
	Vector sphereInter;
	Color *image;
	int xs, ys;

	Sphere();
	Sphere(Vector,double);
	Sphere(Vector,double,Material);
	double sphereIntersection(Ray);
  
	double findIntersection(Ray ray, int intersectionSide);
};

#endif