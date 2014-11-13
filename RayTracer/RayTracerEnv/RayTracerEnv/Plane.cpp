#include "Plane.h"

Plane::Plane()
{
	distance   = 0.0;
	normal      = Vector(1.0,0.0,0.0);
	mat.matColor = Color(0.5,0.5,0.5);
}
Plane::Plane(Vector n,double d)
{
	distance     = d;
	normal       = n;
	mat.matColor = Color(0.5,0.5,0.5);
}
Plane::Plane(Vector n,double d,Material m)
{
	distance = d;
	normal   = n;
	mat      = m;
}

double Plane::findIntersection(Ray r)
{
	/* Formula for intersection(t):
	   t = -V0/Vd ; Vd = Pn.Rd; 
	   V0 = (R0-D).Pn = (R0.Pn)-D.Pn; discard when Vd=0,t<0, return -1
	   if Vd = 0 then ray is parallel to plane
	   also if t<0 points are behind the ray origin */ 
	double V0,Vd,t;
	Vector Pn = normal;    //Plane Normal
	double D  = distance;  // Plane Distance(Vector) from origin 
	Vector R0 = r.origin;  // Ray Origin
	Vector Rd = r.direction;
	Rd = Rd.normalize();
	Vd = Pn.dotProduct(Rd);
	if (Vd == 0){ return -1;} // We can check for negative values and discard later
	else
	{
		V0 = Pn.dotProduct(R0.addVector(Pn.scalarMult(-1*D)));
		//V0 = Pn.dotProduct(R0) + D;
		//V0 = (R0.subVector(Vector(D,D,D))).dotProduct(Pn);
		t  = -1 * (V0/Vd);
		if(t > 0){ return t;}
		else{ return -1;}
	}
}