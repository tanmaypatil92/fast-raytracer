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
  return planeIntersection(r);
}

double Plane::planeIntersection(Ray r)
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
	if (Vd == 0){ t = -1;} // We can check for negative values and discard later
	else
	{
		V0 = Pn.dotProduct(R0.addVector(Pn.scalarMult(-1*D)));
		//V0 = Pn.dotProduct(R0) + D;
		//V0 = (R0.subVector(Vector(D,D,D))).dotProduct(Pn);
		t  = -1 * (V0/Vd);
		if (t < 1)t = -1;
		else
		{
			intersection = R0.addVector(Rd.scalarMult(t));
		}
	}
  
    /* @todo - deprecated, use the superclass properties instead */
  
	this->normal          = Pn.normalize(); //@todo : Uthara, please confirm this.
	// ya both should be normalized
	// but normalizinf intersection messes with the checker pattern
	this->intersection    = R0.addVector(Rd.scalarMult(t));
	int columValue = this->intersection.x;
	int rowvalue   = this->intersection.z;
	if((columValue+rowvalue) %2 == 0)
		mat.matColor = Color(0.5,0.1,0);
	else
		mat.matColor  = Color(0.3,0.3,0.6);
 
  /* Now set the superclass properties.*/
  intersectionValue = t;
  objNormal          = this->normal;
  objIntersection    = this->intersection;//.normalize();
  material.matColor  = mat.matColor;
  return t;
}