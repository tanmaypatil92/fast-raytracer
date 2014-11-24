#include "triangle.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "math.h"

using namespace std;
Triangle::Triangle()
{
	v0 = Vector( 0,  0, 0);                                                                                                                  
    v1 = Vector( 1,  0, 0);                                                                                                                  
    v2 = Vector( 1,  1, 0); 

	mat.matColor = Color(1.0,0.0,0.0);
}
Triangle::Triangle(Vector v0_passed,Vector v1_passed, Vector v2_passed)
{
	v0 = v0_passed;
	v1 = v1_passed;
	v2 = v2_passed;

	bb = BoundingBox();
	bb.SetBounds_Triangle(v0,v1,v2);

	mat.matColor = Color(1.0,0.0,0.0);
}
Vector Triangle::Vertex_to_Vector(Vertex vertex)
{
	Vector vector;

	vector.x = vertex.x;
	vector.y = vertex.y;
	vector.z = vertex.z;
	return vector;
}
Triangle::Triangle(OBJObject obj, int  face_no)
{
	Face f = obj.getFace(face_no);
	if(f.numVertices > 4 )
	{
		cout << "MORE THAN 3 VERTICES." << endl;

		v0 = v1 = v2 = Vector( 0,  0, 0); 
	}
	else
	{
		int vertex_no;
		Vertex vertex;

		vertex_no = f.faceVList[0];
		vertex = obj.getVertex(vertex_no);
		v0 = Vertex_to_Vector(vertex);

		vertex_no = f.faceVList[1];
		vertex = obj.getVertex(vertex_no);
		v1 = Vertex_to_Vector(vertex);

		vertex_no = f.faceVList[2];
		vertex = obj.getVertex(vertex_no);
		v2 = Vertex_to_Vector(vertex);
	}
}

#define ABS(x) (x < 0? (-x) : (x) )

float areaTriangle(Vector v1,Vector v2,Vector v3)
{
	Vector AB = sub(v2,v1);
	Vector AC = sub(v3,v1);

	// AB coordinates
	float x1 = AB.x ; float x2 = AB.y;  float x3 = AB.z;
	// AC coordinates
	float y1 = AC.x ; float y2 = AC.y;  float y3 = AC.z;

  #ifdef OS_X_ENV
    float temp = (x2*y1-x1*y2) + (x3*y2-x2*y3)+ (x1*y3-x3*y1);
    float area = ABS(temp);
    area = area/2.0;
  #else
    float area = abs(( (x2*y1-x1*y2) + (x3*y2-x2*y3)+ (x1*y3-x3*y1) )  / 2.0);
	#endif
  
  return area;
}
double Triangle::findIntersection(Ray ray)
{
	return this->triIntersection(ray);
}
double Triangle::triIntersection(Ray ray)
{
	double intersectionVal = 0.0;

	//------  COMPUTE PLANE NORMALS -----------//
    Vector A, B;
    A = sub(v1 , v0);
    B = sub(v2 , v0);
    
    Vector N = cross(A, B);
	triNormal = N;

    // CHECK IF PLANE IS PARALLEL --------------//
	float NdotRayDirection = dot(N, ray.direction);
	if (NdotRayDirection == 0){ return false; }

	// OBTAIN 
    float d = - dot(N, v0);
	float t = -(dot(N, ray.origin) + d) / NdotRayDirection;
    
	intersectionVal = t;

	// CHECK IF TRINAGLE IS BEHIND RAY ORIGIN
	if (t < 0.01) { intersectionVal = -1; }
    
	
	// COMPUTE INTERSECTION POINT
	Vector P = add(ray.origin , ray.direction.scalarMult(t));
	triInter = P;
    //--------------------- INSIDE OUTSIDE TEST - LEE ---------------------------------
	
    Vector C; // vector perpendicular to triangle's plane
 
    // edge 0
    Vector edge0 = sub(v1,v0);
    Vector VP0 = sub(P , v0);
    C = cross(edge0, VP0);
    if (dot(N, C) < 0)
        intersectionVal = -1;
 
    // edge 1
    Vector edge1 = sub(v2,v1);
    Vector VP1 = sub(P , v1);
    C = cross(edge1, VP1);
    if (dot(N, C) < 0)
        intersectionVal = -1;
 
    // edge 2
    Vector edge2 = sub(v0 , v2);
    Vector VP2 = sub(P , v2);
    C = cross(edge2, VP2);
    if (dot(N, C) < 0)
        intersectionVal = -1;
 
    /* Now set the superclass properties.*/
	  intersectionValue = intersectionVal;
	  objNormal          = triNormal;
	  objIntersection    = triInter;
	
	return intersectionVal;
	

	/*
	//------------ INSIDE OUTSIDE TEST - BARYCENTRIC  - NOT WORKING --------------//
	float area = areaTriangle(v0,v1,v2);
	float a0,a1,a2;
	a0 = areaTriangle(v0,P,v1)/area;
	a1 = areaTriangle(v1,P,v2)/area;
	a2 = areaTriangle(v2,P,v0)/area;

	if((a0+a1+a2) > 0.99 && (a0+a1+a2) < 1.01)
		return true;
	else
		return false;
	*/
}
