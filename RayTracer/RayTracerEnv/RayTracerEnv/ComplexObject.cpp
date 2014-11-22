
#include "ComplexObject.h"

#include <cstdlib>
#include <iostream>
#include <fstream>

#include "math.h"
#include "Transform.h"

using namespace std;
ComplexObject::ComplexObject()
{ 
	myOBJ = OBJObject();
	//myOBJ.parse("cube.obj");

	box = BoundingBox();
	//box.SetBounds_ComplexObject(myOBJ);
	
	mat.matColor = Color(0.8,0.5,0.5);
	material = mat;
}
Oct* ComplexObject::initialize_oct(BoundingBox box)
{
	Oct* ptr_oct = new Oct();

	ptr_oct->bb = box;

	// ASSIGNING TRIANGLES INSIDE EACH OCT TREE
	
	if(curr_oct_depth == max_oct_depth)
	{
		//ptr_oct->tri_index_list = new int(myOBJ.getNumOfFaces());

		ptr_oct->tri_index_list = (int*)malloc(sizeof(int)*n_tri);

		int curr_triList_index = 0;
		for(int t=0;t<n_tri;t++)
		{
			ptr_oct->tri_index_list[curr_triList_index] = -1;
				curr_triList_index++;
		}

		curr_triList_index = 0;
		for(int t=0;t<n_tri;t++)
		{
			if( ptr_oct->bb.Contains(tri_list[t]->bb) )
			{
				ptr_oct->tri_index_list[curr_triList_index] = t;
				curr_triList_index++;
			}
		}
		ptr_oct->n_tri_inside = curr_triList_index;
	}

	if(curr_oct_depth >= max_oct_depth)
		return ptr_oct;

	curr_oct_depth++;
	//
	float x_low = box.left_bound;
	float x_mid = (box.left_bound + box.right_bound)/2.0;
	float x_high = box.right_bound;

	float y_low = box.bottom_bound;
	float y_mid = (box.bottom_bound + box.top_bound)/2.0;
	float y_high = box.top_bound;

	float z_low = box.front_bound;
	float z_mid = (box.front_bound + box.back_bound)/2.0;
	float z_high = box.back_bound;

	// Oct assignment
	// 0  1 
	// 2  3
	// same way 4 5 6 7 behind them



	BoundingBox sub_box[8];

	sub_box[0].SetBounds(x_low, x_mid, y_mid, y_high, z_low, z_mid); sub_box[1].SetBounds(x_mid, x_high, y_mid, y_high, z_low, z_mid);
	sub_box[2].SetBounds(x_low, x_mid, y_low, y_mid, z_low, z_mid);  sub_box[3].SetBounds(x_mid, x_high, y_low, y_mid, z_low, z_mid); 

	sub_box[4].SetBounds(x_low, x_mid, y_mid, y_high, z_mid, z_high); sub_box[5].SetBounds(x_mid, x_high, y_mid, y_high, z_mid, z_high);
	sub_box[6].SetBounds(x_low, x_mid, y_low, y_mid, z_mid, z_high);  sub_box[7].SetBounds(x_mid, x_high, y_low, y_mid, z_mid, z_high);

	for(int i=0;i<8;i++)
		ptr_oct->sub_oct[i] = initialize_oct(sub_box[i]);

	return ptr_oct;
}
void ComplexObject::get_triangles()
{
	n_tri = 0;
	/*
	if(myOBJ.getNumOfFaces() > MAX_TRI)
	{	
		cout << "MORE FACES THAN MAX SIZE OF TRI." << endl;
		getchar();
		exit(1);
	}*/
	
	//tri_list = (Triangle*)(  malloc(sizeof(Triangle)*myOBJ.getNumOfFaces()) );
	
	// LOOP THROUGH EACH POLYGON OF THE COMPLEX OBJECT
	for(int face_no= 0; face_no < myOBJ.getNumOfFaces()  ; face_no++)
	{
				
		Face f = myOBJ.getFace(face_no);
		if(f.numVertices > 4 ) { cout << "MORE THAN 3 VERTICES: NOT HANDELED CURRENTLY." << endl; continue; }

		// PASS VERTICES EXTRACTED FROM OBJ FACE TO CREATE TRIANGLE
		Vector v0, v1, v2;
		v0 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[0]));
		v1 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[1]));
		v2 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[2]));
		

		tri_list[n_tri++] = new Triangle(v0,v1,v2);
			
	}
}
ComplexObject::ComplexObject(char* filename, Material m)
{
	myOBJ = OBJObject();
	myOBJ.parse(filename);
	
	box = BoundingBox();
	box.SetBounds_ComplexObject(myOBJ);
	
	get_triangles();
	
	curr_oct_depth = 0; max_oct_depth = 2;
	
	head_oct = initialize_oct(box);
	
	mat = m;
	material = mat;
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
	//return this->complexIntersection(ray);

	return this->complexIntersection_faster(ray);
}
double ComplexObject::complexIntersection(Ray ray)
{
	double intersectionVal= 0;

	if(!box.Intersection(ray))
	{
		intersectionValue = -1;
		return -1;
	}
	
	double min_intersectionVal  = FLT_MAX;
	Vector min_complexNormal, min_complexInter;

	bool intersects = false;
	int no_of_intersections = 0;

	// LOOP THROUGH EACH POLYGON OF THE COMPLEX OBJECT
	for(int face_no= 0; face_no < myOBJ.getNumOfFaces()  ; face_no++)
	{
				
		Face f = myOBJ.getFace(face_no);
		if(f.numVertices > 4 ) { cout << "MORE THAN 3 VERTICES: NOT HANDELED CURRENTLY." << endl; continue; }

		// PASS VERTICES EXTRACTED FROM OBJ FACE TO CREATE TRIANGLE
		Vector v0, v1, v2;
		v0 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[0]));
		v1 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[1]));
		v2 = Vertex_to_Vector(myOBJ.getVertex(f.faceVList[2]));
		
		Triangle tri = Triangle(v0,v1,v2);
				
		double tri_intersect = tri.triIntersection(ray);
				
		if( (tri_intersect >= 0.0000001) &&  (tri_intersect < min_intersectionVal) )
		{	
			min_intersectionVal = tri_intersect;
			min_complexNormal = tri.triNormal;
			min_complexInter  = tri.triInter;

			intersects = true;

			no_of_intersections++;
			if(no_of_intersections >= 2) {break;}
		}
	}

	intersectionVal = min_intersectionVal;
	if(!intersects) {intersectionVal = -1;}

	/* Now set the superclass properties.*/
	intersectionValue = intersectionVal;
	objNormal          = min_complexNormal.normalize();
	objIntersection    = min_complexInter;

	return min_intersectionVal;
}

#if 0
double ComplexObject::complexIntersection_fast(Ray ray)
{
	// -- ELIMINATE ON BASIS OF TOTAL BOUNDING BOX FIRST
	double intersectionVal= 0;

	if(!box.Intersection(ray))
	{
		intersectionValue = -1;
		return -1;
	}
	

	float min_intersectionVal  = FLT_MAX;
	Vector min_complexNormal, min_complexInter;

	int no_of_intersections = 0;

	// GO THROUGH EACH OCT
	for(int o=0; o<8; o++)
	{
		// CHECK IF OCT IS INTERSECTED, SKIP IF NOT
		if( !oct[o].bb.Intersection(ray) )
			continue;

		// LOOP THROUGH ALL OCT TRINAGLES
		for(int t=0; t<oct[o].n_tri_inside ; t++)
		{
			int tri_list_index =  oct[o].tri_index_list[t];
			Triangle tri = tri_list[tri_list_index];
			
			double tri_intersect = tri.triIntersection(ray);

			if( (tri_intersect >= 0) &&  (tri_intersect < min_intersectionVal) )
			{	
				min_intersectionVal = tri_intersect;
				min_complexNormal = tri.triNormal;
				min_complexInter  = tri.triInter;

				no_of_intersections++;
				if(no_of_intersections >= 2) {break;}
			}
		}

		if(no_of_intersections >= 2) {break;}
	}
	

	intersectionVal = min_intersectionVal;
	if(no_of_intersections == 0) {intersectionVal = -1;}

	/* Now set the superclass properties.*/
	intersectionValue = intersectionVal;
	objNormal          = min_complexNormal;
	objIntersection    = min_complexInter;

	return min_intersectionVal;
}
#endif

double ComplexObject::complexIntersection_faster(Ray ray)
{
	return_data r = complexIntersection_recursive(head_oct ,ray);
	
	if(r.tri_index == -1)
	{
		intersectionValue = -1;
		return -1;
	}
	else
	{
		Triangle* tri = tri_list[r.tri_index];
		double tri_intersect = tri->triIntersection(ray);

		intersectionValue =  tri_intersect;
		objNormal          =  tri->triNormal;
		objIntersection    = tri->triInter;

		return tri_intersect;
	}
}
return_data ComplexObject::complexIntersection_recursive(Oct* oct ,Ray ray)
{
	// IF NOT LEAF NODE RETURN BEST OF ALL SUB OCTS
	if (oct->sub_oct[0] != NULL)
	{
		return_data best_r;
		
		for(int i=0;i<8;i++)
		{
			if( !oct->sub_oct[i]->bb.Intersection(ray) )
				continue;

			return_data r = complexIntersection_recursive(oct->sub_oct[i] ,ray);
			
			if(r.tri_index == -1)
				continue;

			if(r.min_intersectionVal < best_r.min_intersectionVal)
			{
				best_r = r;
			}
		}

		return best_r;
	}
	// ELSE IF NODE : TRAVERSE THROUGH ALL TRIANGLES
	else
	{
		return_data best_r;
		int no_of_intersections = 0;

		// LOOP THROUGH ALL OCT TRINAGLES
		for(int t=0; t< oct->n_tri_inside ; t++)
		{
			int tri_list_index =  oct->tri_index_list[t];
			Triangle* tri = tri_list[tri_list_index];
			
			double tri_intersect = tri->triIntersection(ray);

			if( (tri_intersect > 0) &&  (tri_intersect < best_r.min_intersectionVal) )
			{	
				
				best_r.tri_index = tri_list_index;
				best_r.min_intersectionVal = tri_intersect;

				no_of_intersections++;
				if(no_of_intersections >= 2) {break;}
			}
		}

		return best_r;
	}
}