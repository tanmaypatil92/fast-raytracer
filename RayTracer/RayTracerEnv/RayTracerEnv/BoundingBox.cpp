#include "BoundingBox.h"
#include "math.h"
#include <limits>

BoundingBox::BoundingBox()
{
	left_bound = FLT_MAX;	top_bound = FLT_MAX; front_bound = FLT_MAX;
	right_bound = -FLT_MAX;	bottom_bound = -FLT_MAX; back_bound = -FLT_MAX;
}
void BoundingBox::SetBounds_Sphere(Vector center, double radius)
{
	left_bound = center.x - radius;
	right_bound = center.x + radius;

	top_bound = center.y - radius;
	bottom_bound = center.y + radius;

	front_bound = center.y - radius;
	back_bound = center.y + radius;
}
void BoundingBox::SetBounds_ComplexObject(OBJObject myOBJ)
{
	left_bound = FLT_MAX;	bottom_bound = FLT_MAX; front_bound = FLT_MAX;
	right_bound = -FLT_MAX;	top_bound = -FLT_MAX; back_bound = -FLT_MAX;

	int num_vertices = myOBJ.getNumOfVertices();

	for(int i=1;i<=num_vertices;i++)
    {
		Vertex v = myOBJ.getVertex(i);
		
		if(v.x < left_bound) {left_bound = v.x;}
		if(v.x > right_bound) {right_bound = v.x;}

		if(v.y < bottom_bound) {bottom_bound = v.y;}
		if(v.y > top_bound) {top_bound = v.y;}
		
		if(v.z < front_bound) {front_bound = v.z;}
		if(v.z > back_bound) {back_bound = v.z;}

    }
}

bool BoundingBox::Intersection(Ray r)
{
	//Reference : http://tavianator.com/2011/05/fast-branchless-raybounding-box-intersections/
	//http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm

	float t_near_furtherest = -FLT_MAX, t_far_nearest = +FLT_MAX;
	float t_near, t_far;
	float t1, t2;

	//------ X BOUNDING PLANES
	if(r.direction.x == 0) // PARALLEL TO X BOUNDING PLANES
	{
		if(r.origin.x >= left_bound && r.origin.x <= right_bound)
			return true;
		else
			return false;
	}

	t1 = (-1 - r.origin.x) / r.direction.x;
	t2 = ( 1 - r.origin.x) / r.direction.x;
	if(t1 <= t2) {t_near = t1; t_far = t2;}
	else {t_near = t2; t_far = t1;}
	if(t_near > t_near_furtherest)   {t_near_furtherest = t_near;}
	if(t_far < t_far_nearest) {t_far_nearest = t_far;}
	
	//------ Y BOUNDING PLANES
	if(r.direction.y == 0) // PARALLEL TO Y BOUNDING PLANES
	{
		if(r.origin.y >= bottom_bound && r.origin.y <= top_bound)
			return true;
		else
			return false;
	}

	t1 = (-1 - r.origin.y) / r.direction.y;
	t2 = ( 1 - r.origin.y) / r.direction.y;
	if(t1 <= t2) {t_near = t1; t_far = t2;}
	else {t_near = t2; t_far = t1;}
	if(t_near > t_near_furtherest)   {t_near_furtherest = t_near;}
	if(t_far < t_far_nearest) {t_far_nearest = t_far;}

	//------ Z BOUNDING PLANES
	if(r.direction.z == 0) // PARALLEL TO Z BOUNDING PLANES
	{
		if(r.origin.z >= front_bound && r.origin.z <= back_bound)
			return true;
		else
			return false;
	}

	t1 = (-1 - r.origin.z) / r.direction.z;
	t2 = ( 1 - r.origin.z) / r.direction.z;
	if(t1 <= t2) {t_near = t1; t_far = t2;}
	else {t_near = t2; t_far = t1;}
	if(t_near > t_near_furtherest)   {t_near_furtherest = t_near;}
	if(t_far < t_far_nearest) {t_far_nearest = t_far;}

	//------------------------------------------

	if(t_near_furtherest > t_far_nearest)
		return false;
	else
		return true;
}
