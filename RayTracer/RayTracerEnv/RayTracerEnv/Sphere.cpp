#include "Sphere.h"
#include "math.h"

#define TRACER_THRESH   0.0000001

Sphere::Sphere()
{
	center = Vector (0.0,0.0,0.0);
	radius = 1.0;
	mat.matColor = Color(0.5,0.5,0.5);
  objectType = 1; //1 for sphere
}

Sphere::Sphere(Vector c,double r)
{
	center = c;
	radius = r;
	mat.matColor = Color(0.5,0.5,0.5);
  material.matColor = mat.matColor;
  objectType = 1; //1 for sphere
}

Sphere::Sphere(Vector c,double r,Material m)
{
	center = c;
	radius = r;
	mat    = m;
  material = m;
  objectType = 1; //1 for sphere
}

double Sphere::findIntersection(Ray ray, int intersectionSide)
{

  return this->sphereIntersection(ray);
}

double Sphere::sphereIntersection(Ray r)
{
	// Sphere Equation (Xs-Xc)^2+(Ys-Yc)^2+(Zs-Zc)^2=r^2
	Vector R0 = r.origin;  // Ray Origin
	Vector Rd = r.direction;
	double A,B,C;
	double discriminant,rootA,rootB;
	double intersectionVal = 0.0;
	A = (Rd.x * Rd.x) + (Rd.y* Rd.y) + (Rd.z * Rd.z);
	B = 2 * (Rd.x*(R0.x - center.x) + Rd.y*(R0.y - center.y) + Rd.z*(R0.z - center.z));
	C = ((R0.x - center.x)*(R0.x - center.x) + (R0.y - center.y)*(R0.y - center.y) + (R0.z - center.z)*(R0.z - center.z)) - (radius*radius);
	discriminant = (B*B - 4*A*C);
	if(discriminant < 0)  {
    intersectionVal = -1;
  }
  else if (discriminant == 0) {
    rootA = (-B / (2 * A));
    intersectionVal = rootA;
  }
	else
	{
		rootA = ((-B)-sqrt(discriminant))/(2.0 * A);
		if(rootA > 0 && rootA > TRACER_THRESH)
		{
			intersectionVal = rootA;
		}
		else
		{
			rootB = ((-B)+sqrt(discriminant))/(2.0 * A);
			if(rootB > 0 && rootB > TRACER_THRESH)
			{
				intersectionVal = rootB;
			}
			else{intersectionVal = -1;}
		}
	}

  //sphereInter  = (R0.addVector(Rd.scalarMult(intersectionVal))).normalize();
	//sphereNormal = ((sphereInter.subVector(center)).normalize()).scalarMult(1/radius);
	Vector temp = Vector((R0.x - center.x),(R0.y - center.y),(R0.z - center.z));
  /* @todo - deprecated, use the superclass properties instead */
	sphereNormal   = (temp.addVector(Rd.scalarMult(intersectionVal))).normalize();
	//sphereInter    = R0.addVector(Rd.scalarMult(intersectionVal)).normalize();
  sphereInter    = R0.addVector(Rd.scalarMult(intersectionVal));

  /* Now set the superclass properties.*/
  objNormal         = sphereNormal;
  objIntersection   = sphereInter;

  intersectionValue = intersectionVal;
  return intersectionVal;
}


