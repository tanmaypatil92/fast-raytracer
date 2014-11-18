
#include "ComplexObject.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

#include "math.h"

using namespace std;
ComplexObject::ComplexObject()
{ 
	myOBJ = OBJObject();
	myOBJ.parse("cube.obj");
	mat.matColor = Color(0.8,0.5,0.5);
}
ComplexObject::ComplexObject(char* filename, Material m)
{
	myOBJ = OBJObject();
	myOBJ.parse(filename);
	mat = m;
}
Vector ComplexObject::Vertex_to_Vector(Vertex vertex)
{
	Vector vector;

	vector.x = vertex.x;
	vector.y = vertex.y;
	vector.z = vertex.z;
	return vector;
}
void ComplexObject::OutputFaces()
{
	int num_face = myOBJ.getNumOfFaces();
    cout << "Found " << num_face << " faces..." << endl;
    
	for(int i=0;i<num_face;i++)
    {
        Face  f = myOBJ.getFace(i);
        cout << "Face: " << i << " ";
        for(int y=0; y < f.numVertices; y++)
        {
            int currentVertex = f.faceVList[y];
            cout << currentVertex << "->";
        }
        cout << f.faceVList[0] << endl; 
    }

	getchar();
}
double ComplexObject::findIntersection(Ray ray)
{
	return this->complexIntersection(ray);
}
double ComplexObject::complexIntersection(Ray ray)
{
	double intersectionVal= 0;

	float min_intersectionVal  = FLT_MAX;
	Vector min_complexNormal, min_complexInter;

	bool intersects = false;

	// LOOP THROUGH EACH POLYGON OF THE COMPLEX OBJECT
	for(int face_no= 0; face_no < myOBJ.getNumOfFaces()  ; face_no++)
	{
		// FOR PROGRESS CHECK
		//cout << y<< "\t" << x << "\t" << face_no<<endl;
				
		Face f = myOBJ.getFace(face_no);
		if(f.numVertices > 4 ) { cout << "MORE THAN 3 VERTICES: NOT HANDELED CURRENTLY." << endl; continue; }

		// PASS VERTICES EXTRACTED FROM OBJ FACE TO CREATE TRIANGLE
		Vector v0, v1, v2;
		v0 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[0]));
		v1 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[1]));
		v2 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[2]));
		
		Triangle tri = Triangle(v0,v1,v2);
				
		double tri_intersect = tri.triIntersection(ray);
				
		if( (tri_intersect >= 0) &&  (tri_intersect < min_intersectionVal) )
		{	
			min_intersectionVal = tri_intersect;
			min_complexNormal = tri.triNormal;
			min_complexInter  = tri.triInter;

			intersects = true;
		}
	}

	intersectionVal = min_intersectionVal;
	if(!intersects) {intersectionVal = -1;}

	/* Now set the superclass properties.*/
	intersectionValue = intersectionVal;
	objNormal          = min_complexNormal;
	objIntersection    = min_complexInter;

	return min_intersectionVal;
}
